//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "Equalizer.h"

Equalizer::Equalizer(vector<Float32> &fVector, vector<Float32> &qVector, vector<Float32> &gainVector, Float32 sampleRate) {
    if (fVector.size() != qVector.size() || fVector.size() != gainVector.size()) {
        throw std::invalid_argument("Equalizer vectors must be of the same size.");
    }

    for (int i = 0; i < fVector.size(); i++) {
        vector<Float32> a;
        vector<Float32> b;

        calculatePeakFilter(fVector[i], qVector[i], gainVector[i], sampleRate, a, b);

        filters.push_back(IIRFilter(a, b));
    }
}

vector<Float32> Equalizer::process(vector<Float32> input) {
    vector<Float32> output = input;
    for (auto& filter : filters) {
        output = filter.processBlock(output);
    }
    return output;
}

void Equalizer::calculatePeakFilter(Float32 f, Float32 q, Float32 gain, Float32 sampleRate, vector<Float32>& a, vector<Float32>& b) {
    Float32 A = pow(10, gain / 40.0f);
    Float32 omega = 2.0f * M_PI * f / sampleRate;
    Float32 alpha = sin(omega) / (2.0f * q);

    Float32 a0 = 1.0f + alpha / A;
    Float32 a1 = -2.0f * cos(omega);
    Float32 a2 = 1.0f - alpha / A;
    Float32 b0 = 1.0f + alpha * A;
    Float32 b1 = -2.0f * cos(omega);
    Float32 b2 = 1.0f - alpha * A;

    a = {a0, a1, a2};
    b = {b0, b1, b2};

    for (auto& coeff : a) {
        coeff /= a0;
    }
    for (auto& coeff : b) {
        coeff /= a0;
    }
}