//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_IIRFILTER_H
#define EQ_CPP_IIRFILTER_H

#include <map>
#include <vector>
#include <stdexcept>

class IIRFilter {
public:
    float f;
    float q;
    float g;

    IIRFilter(float f, float q, float g, float sampleRate);

    void processBlock(std::vector<float>& input);

    void calculatePeakFilter(float f, float q, float g, float sampleRate, std::vector<double>& a, std::vector<double>& b);
private:
    std::vector<double> a_coeffs;
    std::vector<double> b_coeffs;
    std::vector<double> state;
};


#endif //EQ_CPP_IIRFILTER_H
