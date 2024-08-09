//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "convolutionReverb.h"
#include "iostream"


ConvolutionReverb::ConvolutionReverb(std::string path, float dryWet) : path(path), dryWet(Smoother(dryWet, dryWet, 0)) {
    chunkSize = 16384;
    paddedSize = chunkSize * 2;

    fftSetup = vDSP_create_fftsetup(log2f(paddedSize), FFT_RADIX2);

    impulseResponse = readIRFile(path);
    impulseResponseFFTs.resize(ceil(static_cast<float>(impulseResponse.size() / chunkSize)));
    impulseResponse.resize(impulseResponseFFTs.size() * chunkSize, 0);
    for (size_t i = 0; i < impulseResponseFFTs.size(); ++i) {
        std::vector<float> chunked = std::vector<float>(impulseResponse.begin() + i * chunkSize, impulseResponse.begin() + (i + 1) * chunkSize);
        chunked.resize(paddedSize, 0);
        impulseResponseFFTs[i] = fft(chunked);
    }

    overlap.resize(impulseResponseFFTs.size() * chunkSize, 0);
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
    size_t totalSize = (impulseResponseFFTs.size() + 1) * (chunkSize);

    input.resize(paddedSize, 0);
    std::vector<std::complex<float>> inputFFT = fft(input);

    std::vector<float> totalInverseFFT(totalSize, 0);
    for (size_t i = 0; i < impulseResponseFFTs.size(); ++i) {
        std::vector<std::complex<float>> convolved(inputFFT.size());
        for (size_t j = 0; j < inputFFT.size(); ++j) {
            convolved[j] = inputFFT[j] * impulseResponseFFTs[i][j];
        }

        std::vector<float> inverseFFT = ifft(convolved);

        for (size_t k = 0; k < inverseFFT.size(); ++k) {
            totalInverseFFT[k + i * chunkSize] += inverseFFT[k];
        }
    }

    std::vector<float> output(inputSize);
    for (size_t i = 0; i < totalInverseFFT.size(); ++i) {
        overlap[i] += totalInverseFFT[i];
    }
    std::copy(overlap.begin(), overlap.begin() + inputSize, output.begin());
    overlap.erase(overlap.begin(), overlap.begin() + inputSize);
    overlap.resize(totalSize);

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