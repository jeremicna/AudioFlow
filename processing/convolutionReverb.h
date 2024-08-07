//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_CONVOLUTIONREVERB_H
#define EQ_CPP_CONVOLUTIONREVERB_H

#include <map>
#include <Accelerate/Accelerate.h>
#include "../fileutils/readIRFile.h"
#include "smoother.h"

class ConvolutionReverb {
public:
    std::string path;

    ConvolutionReverb(std::string path, float dryWet);

    std::vector<std::complex<float>> fft(const std::vector<float> input);
    std::vector<float> ifft(std::vector<std::complex<float>> input);

    void process(std::vector<float>& input);

    float getDryWet();
    void setDryWet(float dryWet);
private:
    Smoother dryWet;
    size_t fftSize;
    FFTSetup fftSetup;
    std::vector<float> overlap;
    std::vector<float> impulseResponse;
    std::vector<std::complex<float>> impulseResponseFFT;
};


#endif //EQ_CPP_CONVOLUTIONREVERB_H
