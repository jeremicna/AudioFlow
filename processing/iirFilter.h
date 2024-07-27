//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_IIRFILTER_H
#define EQ_CPP_IIRFILTER_H

#include <map>
#include <vector>
#include <stdexcept>


using namespace std;

class IIRFilter {
public:
    IIRFilter(vector<float>& a, vector<float>& b);

    void processBlock(vector<float>& input);
private:
    vector<float> a_coeffs;
    vector<float> b_coeffs;
    vector<float> state;
};


#endif //EQ_CPP_IIRFILTER_H
