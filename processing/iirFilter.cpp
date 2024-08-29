//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "iirFilter.h"

IIRFilter::IIRFilter(float f, float q, float g, float sampleRate)
    : frequency(Smoother(f, f, 0)), quality(Smoother(q, q, 0)), gain(Smoother(g, g, 0)), sampleRate(Smoother(sampleRate, sampleRate, 0)) {

    calculatePeakFilter();
    state = std::vector<double>(a_coeffs.size(), 0.0);
}

void IIRFilter::process(std::vector<float>& input) {
    for (size_t n = 0; n < input.size(); ++n) {
        if (frequency.getRemaining() > 0 || quality.getRemaining() > 0 || gain.getRemaining() > 0) {
            calculatePeakFilter();
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

void IIRFilter::calculatePeakFilter() {
    double A = pow(10.0, gain.currentValue() / 40.0);
    double omega = 2.0 * M_PI * frequency.currentValue() / sampleRate.currentValue();
    double alpha = sin(omega) / (2.0 * quality.currentValue());

    double a0 = 1.0 + alpha / A;
    double a1 = -2.0 * cos(omega);
    double a2 = 1.0 - alpha / A;
    double b0 = 1.0 + alpha * A;
    double b1 = -2.0 * cos(omega);
    double b2 = 1.0 - alpha * A;

    a_coeffs = {a0, a1, a2};
    b_coeffs = {b0, b1, b2};

    for (auto& coeff : a_coeffs) {
        coeff /= a0;
    }
    for (auto& coeff : b_coeffs) {
        coeff /= a0;
    }
}

float IIRFilter::getFrequency() {
    return frequency.currentValueNoChange();
}

void IIRFilter::setFrequency(float newFrequency) {
    frequency = Smoother(frequency.currentValueNoChange(), newFrequency, smootherSteps);
}

float IIRFilter::getQuality() {
    return quality.currentValueNoChange();
}

void IIRFilter::setQuality(float newQuality) {
    quality = Smoother(quality.currentValueNoChange(), newQuality, smootherSteps);
}

float IIRFilter::getGain() {
    return gain.currentValueNoChange();
}

void IIRFilter::setGain(float newGain) {
    gain = Smoother(gain.currentValueNoChange(), newGain, smootherSteps);
}