//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "iirFilter.h"


IIRFilter::IIRFilter(float f, float q, float g, float sampleRate)
    : f(f), q(q), g(g) {

    calculatePeakFilter(f, q, g, sampleRate, a_coeffs, b_coeffs);
    state = std::vector<double>(a_coeffs.size(), 0.0);

    if (a_coeffs[0] != 1.0) {
        throw std::invalid_argument("The first coefficient of a must be 1.");
    }
}

void IIRFilter::calculatePeakFilter(float f, float q, float g, float sampleRate, std::vector<double>& a, std::vector<double>& b) {
    double A = pow(10.0, g / 40.0);
    double omega = 2.0 * M_PI * f / sampleRate;
    double alpha = sin(omega) / (2.0 * q);

    double a0 = 1.0 + alpha / A;
    double a1 = -2.0 * cos(omega);
    double a2 = 1.0 - alpha / A;
    double b0 = 1.0 + alpha * A;
    double b1 = -2.0 * cos(omega);
    double b2 = 1.0 - alpha * A;

    a = {a0, a1, a2};
    b = {b0, b1, b2};

    for (auto& coeff : a) {
        coeff /= a0;
    }
    for (auto& coeff : b) {
        coeff /= a0;
    }
}


void IIRFilter::processBlock(std::vector<float>& input) {
    for (size_t n = 0; n < input.size(); ++n) {
        double w0 = input[n];
        for (size_t k = 1; k < a_coeffs.size(); ++k) {
            w0 -= a_coeffs[k] * state[k - 1];
        }

        double yn = b_coeffs[0] * w0;
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