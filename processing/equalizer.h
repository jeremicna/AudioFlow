//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_EQUALIZER_H
#define EQ_CPP_EQUALIZER_H

#include <map>
#include "iirFilter.h"

class Equalizer {
public:
    Equalizer(std::vector<float>& fVector, std::vector<float>& qVector, std::vector<float>& gainVector, float);

    void process(std::vector<float>& input);
private:
    std::vector<IIRFilter> filters;

    void calculatePeakFilter(float f, float Q, float gain, float sampleRate, std::vector<double>& a, std::vector<double>& b);
};


#endif //EQ_CPP_EQUALIZER_H
