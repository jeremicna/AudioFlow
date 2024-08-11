//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "iirFilter.h"

IIRFilter::IIRFilter(float f, float q, float g, float sampleRate)
    : f(Smoother(f, f, 0)), q(Smoother(q, q, 0)), g(Smoother(g, g, 0)), sampleRate(Smoother(sampleRate, sampleRate, 0)) {

    calculatePeakFilter(a_coeffs, b_coeffs);
    state = std::vector<double>(a_coeffs.size(), 0.0);
}

void IIRFilter::process(std::vector<float>& input) {
    for (size_t n = 0; n < input.size(); ++n) {
        if (f.getRemaining() > 0 || q.getRemaining() > 0 || g.getRemaining() > 0) {
            calculatePeakFilter(a_coeffs, b_coeffs);
        }

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

void IIRFilter::calculatePeakFilter(std::vector<double>& a, std::vector<double>& b) {
    double A = pow(10.0, g.currentValue() / 40.0);
    double omega = 2.0 * M_PI * f.currentValue() / sampleRate.currentValue();
    double alpha = sin(omega) / (2.0 * q.currentValue());

    double a0 = 1.0 + alpha / A;
    double a1 = -2.0 * cos(omega);
    double a2 = 1.0 - alpha / A;
    double b0 = 1.0 + alpha * A;
    double b1 = -2.0 * cos(omega);
    double b2 = 1.0 - alpha * A;

    a = {a0, a1, a2};
    b = {b0, b1, b2};

    for (auto& coeff : a_coeffs) {
        coeff /= a0;
    }
    for (auto& coeff : b_coeffs) {
        coeff /= a0;
    }
}

float IIRFilter::getF() {
    return f.currentValueNoChange();
}

void IIRFilter::setF(float f) {
    this->f = Smoother(this->f.currentValueNoChange(), f, 256);
}

float IIRFilter::getQ() {
    return q.currentValueNoChange();
}

void IIRFilter::setQ(float q) {
    this->q = Smoother(this->q.currentValueNoChange(), q, 256);
}

float IIRFilter::getG() {
    return g.currentValueNoChange();
}

void IIRFilter::setG(float g) {
    this->g = Smoother(this->g.currentValueNoChange(), g, 256);
}