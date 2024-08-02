//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_EQUALIZER_H
#define EQ_CPP_EQUALIZER_H

#include <map>
#include "iirFilter.h"

class Equalizer {
public:
    std::vector<float> &fVector;
    std::vector<float> &qVector;
    std::vector<float> &gVector;
    float sampleRate;

    Equalizer(std::vector<float>& fVector, std::vector<float>& qVector, std::vector<float>& gVector, float sampleRate);

    void process(std::vector<float>& input);

    std::vector<IIRFilter> getFilters();
    void setFilters(std::vector<IIRFilter> filters);
private:
    std::vector<IIRFilter> filters;
};


#endif //EQ_CPP_EQUALIZER_H
