//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_IIRFILTER_H
#define EQ_CPP_IIRFILTER_H

#include <map>
#include <vector>
#include <stdexcept>
#include "smoother.h"

class IIRFilter {
public:
    IIRFilter(float frequency, float quality, float gain, float sampleRate);

    void process(std::vector<float>& input);

    void calculatePeakFilter();

    float getFrequency();
    void setFrequency(float newFrequency);

    float getQuality();
    void setQuality(float newQuality);

    float getGain();
    void setGain(float newGain);
private:
    Smoother frequency;
    Smoother quality;
    Smoother gain;
    Smoother sampleRate;
    std::vector<double> a_coeffs;
    std::vector<double> b_coeffs;
    std::vector<double> state;
};


#endif //EQ_CPP_IIRFILTER_H
