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
    IIRFilter(float f, float q, float g, float sampleRate);

    void process(std::vector<float>& input);

    void calculatePeakFilter();

    float getF();
    void setF(float gain);

    float getQ();
    void setQ(float gain);

    float getG();
    void setG(float gain);
private:
    Smoother f;
    Smoother q;
    Smoother g;
    Smoother sampleRate;

    std::vector<double> a_coeffs;
    std::vector<double> b_coeffs;
    std::vector<double> state;
};


#endif //EQ_CPP_IIRFILTER_H
