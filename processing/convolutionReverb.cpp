//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "convolutionReverb.h"


ConvolutionReverb::ConvolutionReverb(std::string path, float dryWet) : path(path), dryWet(Smoother(dryWet, dryWet, 0)) {
    impulseResponse = readIRFile(path);
    fftSize = 1 << static_cast<int>(ceil(log2(8192 + impulseResponse.size() - 1)));
    fftSetup = vDSP_create_fftsetup(log2f(fftSize), FFT_RADIX2);
    overlap.resize(8192 + impulseResponse.size() - 1, 0);

    impulseResponse.resize(fftSize, 0);
    impulseResponseFFT = fft(impulseResponse);
}

std::vector<std::complex<float>> ConvolutionReverb::fft(const std::vector<float> input) {
    const size_t n = input.size();

    std::vector<float> real(n / 2);
    std::vector<float> imag(n / 2);
    DSPSplitComplex splitComplex = { real.data(), imag.data() };

    std::vector<float> tempBuffer(input);
    vDSP_ctoz(reinterpret_cast<DSPComplex*>(tempBuffer.data()), 2, &splitComplex, 1, n / 2);

    vDSP_fft_zrip(fftSetup, &splitComplex, 1, log2(n), FFT_FORWARD);

    float scale = 0.5f;
    vDSP_vsmul(splitComplex.realp, 1, &scale, splitComplex.realp, 1, n / 2);
    vDSP_vsmul(splitComplex.imagp, 1, &scale, splitComplex.imagp, 1, n / 2);

    std::vector<std::complex<float>> output(n);

    output[0] = std::complex<float>(splitComplex.realp[0], 0);
    for (size_t i = 1; i < n / 2; ++i) {
        output[i] = std::complex<float>(splitComplex.realp[i], splitComplex.imagp[i]);
        output[n - i] = std::complex<float>(splitComplex.realp[i], -splitComplex.imagp[i]);
    }
    output[n / 2] = std::complex<float>(splitComplex.realp[n / 2], 0);

    return output;
}

std::vector<float> ConvolutionReverb::ifft(std::vector<std::complex<float>> input) {
    const size_t n = input.size();

    std::vector<float> real(n);
    std::vector<float> imag(n);

    for (size_t i = 0; i < input.size(); ++i) {
        real[i] = input[i].real();
        imag[i] = input[i].imag();
    }

    imag[0] = input[n / 2].real();
    DSPSplitComplex splitComplex;
    splitComplex.realp = real.data();
    splitComplex.imagp = imag.data();

    vDSP_fft_zrip(fftSetup, &splitComplex, 1, log2f(n), FFT_INVERSE);

    std::vector<float> output(input.size());
    float* data = output.data();

    vDSP_ztoc(&splitComplex, 1, reinterpret_cast<COMPLEX*>(data), 2, n / 2);
    const float factor = 1.0f / static_cast<float>(n);
    vDSP_vsmul(data, 1, &factor, data, 1, n);

    return output;
}


void ConvolutionReverb::process(std::vector<float>& input) {
    size_t inputSize = input.size();
    input.resize(fftSize, 0);
    std::vector<std::complex<float>> inputFFT = fft(input);

    std::vector<std::complex<float>> convolved(inputFFT.size());
    for (size_t i = 0; i < inputFFT.size(); ++i) {
        convolved[i] = inputFFT[i] * impulseResponseFFT[i];
    }

    std::vector<float> inverseFFT = ifft(convolved);

    std::vector<float> output(inputSize);
    for (size_t i = 0; i < input.size(); ++i) {
        overlap[i] += inverseFFT[i];
    }
    std::copy(overlap.begin(), overlap.begin() + inputSize, output.begin());
    overlap.erase(overlap.begin(), overlap.begin() + inputSize);
    overlap.resize(8192 + impulseResponse.size() - 1);

    for (size_t i = 0; i < output.size(); ++i) {
        float dw = dryWet.currentValue();
        output[i] = (output[i] * dw) + (input[i] * (1 - dw));
    }

    input = output;
}

float ConvolutionReverb::getDryWet() {
    return dryWet.currentValueNoChange();
}

void ConvolutionReverb::setDryWet(float dryWet) {
    this->dryWet = Smoother(this->dryWet.currentValueNoChange(), dryWet, 256);
}