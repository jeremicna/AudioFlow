//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_CONVOLUTIONREVERB_H
#define EQ_CPP_CONVOLUTIONREVERB_H

#include <map>
#include <thread>
#include <Accelerate/Accelerate.h>
#include "../fileutils/readIRFile.h"
#include "smoother.h"

class ConvolutionReverb {
public:
    std::string path;

    ConvolutionReverb(std::string path, float dryWet);

    std::vector<std::complex<float>> fft(const std::vector<float> input, FFTSetup fftSetup);
    std::vector<float> ifft(std::vector<std::complex<float>> input, FFTSetup fftSetup);

    void process(std::vector<float>& input);

    float getDryWet();
    void setDryWet(float dryWet);
private:
    size_t chunkSize;
    size_t paddedSize;
    Smoother dryWet;
    std::vector<FFTSetup> fftSetups;
    std::vector<float> overlap;
    std::vector<float> impulseResponse;
    std::vector<std::vector<std::complex<float>>> impulseResponseFFTs;
};


#endif //EQ_CPP_CONVOLUTIONREVERB_H
