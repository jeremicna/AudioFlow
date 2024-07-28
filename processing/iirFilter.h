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
    IIRFilter(std::vector<double>& a, std::vector<double>& b);

    void processBlock(std::vector<float>& input);
private:
    std::vector<double> a_coeffs;
    std::vector<double> b_coeffs;
    std::vector<double> state;
};


#endif //EQ_CPP_IIRFILTER_H
