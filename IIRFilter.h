//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_IIRFILTER_H
#define EQ_CPP_IIRFILTER_H

#include <map>
#include <CoreAudio/CoreAudio.h>

using namespace std;

class IIRFilter {
public:
    IIRFilter(vector<Float32>& a, vector<Float32>& b);

    vector<Float32> processBlock(vector<Float32> input);
private:
    vector<Float32> a_coeffs;
    vector<Float32> b_coeffs;
    vector<Float32> state;
};


#endif //EQ_CPP_IIRFILTER_H
