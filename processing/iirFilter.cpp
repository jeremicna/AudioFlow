//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "iirFilter.h"


IIRFilter::IIRFilter(vector<float> &a, vector<float> &b)
    : a_coeffs(a), b_coeffs(b), state(vector<float>(a.size(), 0.0)) {

    if (a_coeffs[0] != 1.0) {
        throw std::invalid_argument("The first coefficient of a must be 1.");
    }
}

void IIRFilter::processBlock(vector<float>& input) {
    for (size_t n = 0; n < input.size(); ++n) {
        float w0 = input[n];
        for (size_t k = 1; k < a_coeffs.size(); ++k) {
            w0 -= a_coeffs[k] * state[k - 1];
        }

        float yn = b_coeffs[0] * w0;
        for (size_t k = 1; k < b_coeffs.size(); ++k) {
            yn += b_coeffs[k] * state[k - 1];
        }
        input[n] = yn;

        for (size_t i = state.size() - 1; i > 0; --i) {
            state[i] = state[i - 1];
        }
        if (!state.empty()) {
            state[0] = w0;
        }
    }
}