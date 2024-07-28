//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "equalizer.h"

Equalizer::Equalizer(std::vector<float> &fVector, std::vector<float> &qVector, std::vector<float> &gainVector, float sampleRate) {
    if (fVector.size() != qVector.size() || fVector.size() != gainVector.size()) {
        throw std::invalid_argument("Equalizer vectors must be of the same size.");
    }

    for (int i = 0; i < fVector.size(); i++) {
        std::vector<double> a;
        std::vector<double> b;

        calculatePeakFilter(fVector[i], qVector[i], gainVector[i], sampleRate, a, b);

        filters.push_back(IIRFilter(a, b));
    }
}

void Equalizer::process(std::vector<float>& input) {
    for (auto& filter : filters) {
        filter.processBlock(input);
    }
}

void Equalizer::calculatePeakFilter(float f, float q, float gain, float sampleRate, std::vector<double>& a, std::vector<double>& b) {
    double A = pow(10.0, gain / 40.0);
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