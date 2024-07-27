//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_EQUALIZER_H
#define EQ_CPP_EQUALIZER_H

#include <map>
#include "iirFilter.h"

using namespace std;

class Equalizer {
public:
    Equalizer(vector<float>& fVector, vector<float>& qVector, vector<float>& gainVector, float);

    void process(vector<float>& input);
private:
    vector<IIRFilter> filters;

    void calculatePeakFilter(float f, float Q, float gain, float sampleRate, vector<float>& a, vector<float>& b);
};


#endif //EQ_CPP_EQUALIZER_H
