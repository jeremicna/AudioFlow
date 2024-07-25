//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_EQUALIZER_H
#define EQ_CPP_EQUALIZER_H

#include <CoreAudio/CoreAudio.h>
#include <map>
#include "iirFilter.h"

using namespace std;

class Equalizer {
public:
    Equalizer(vector<Float32>& fVector, vector<Float32>& qVector, vector<Float32>& gainVector, Float32);

    void process(vector<Float32>& input);
private:
    vector<IIRFilter> filters;

    void calculatePeakFilter(Float32 f, Float32 Q, Float32 gain, Float32 sampleRate, vector<Float32>& a, vector<Float32>& b);
};


#endif //EQ_CPP_EQUALIZER_H
