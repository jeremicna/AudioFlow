//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "equalizer.h"

Equalizer::Equalizer(vector<float> &fVector, vector<float> &qVector, vector<float> &gainVector, float sampleRate) {
    if (fVector.size() != qVector.size() || fVector.size() != gainVector.size()) {
        throw std::invalid_argument("Equalizer vectors must be of the same size.");
    }

    for (int i = 0; i < fVector.size(); i++) {
        vector<float> a;
        vector<float> b;

        calculatePeakFilter(fVector[i], qVector[i], gainVector[i], sampleRate, a, b);

        filters.push_back(IIRFilter(a, b));
    }
}

void Equalizer::process(vector<float>& input) {
    for (auto& filter : filters) {
        filter.processBlock(input);
    }
}

void Equalizer::calculatePeakFilter(float f, float q, float gain, float sampleRate, vector<float>& a, vector<float>& b) {
    float A = pow(10, gain / 40.0f);
    float omega = 2.0f * M_PI * f / sampleRate;
    float alpha = sin(omega) / (2.0f * q);

    float a0 = 1.0f + alpha / A;
    float a1 = -2.0f * cos(omega);
    float a2 = 1.0f - alpha / A;
    float b0 = 1.0f + alpha * A;
    float b1 = -2.0f * cos(omega);
    float b2 = 1.0f - alpha * A;

    a = {a0, a1, a2};
    b = {b0, b1, b2};

    for (auto& coeff : a) {
        coeff /= a0;
    }
    for (auto& coeff : b) {
        coeff /= a0;
    }
}