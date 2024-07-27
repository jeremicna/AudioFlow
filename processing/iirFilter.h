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
    IIRFilter(std::vector<float>& a, std::vector<float>& b);

    void processBlock(std::vector<float>& input);
private:
    std::vector<float> a_coeffs;
    std::vector<float> b_coeffs;
    std::vector<float> state;
};


#endif //EQ_CPP_IIRFILTER_H
