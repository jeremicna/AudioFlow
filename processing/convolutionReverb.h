//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_CONVOLUTIONREVERB_H
#define EQ_CPP_CONVOLUTIONREVERB_H

#include <map>
#include <Accelerate/Accelerate.h>

class ConvolutionReverb {
public:
    ConvolutionReverb(std::vector<float> impulseResponse);

    std::vector<std::complex<float>> fft(const std::vector<float> input);
    std::vector<float> ifft(std::vector<std::complex<float>> input);

    void process(std::vector<float>& input);
private:
    size_t fftSize;
    FFTSetup fftSetup;
    std::vector<float> overlap;
    std::vector<float> impulseResponse;
    std::vector<std::complex<float>> impulseResponseFFT;
};


#endif //EQ_CPP_CONVOLUTIONREVERB_H
