//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "Amplifier.h"

Amplifier::Amplifier(Float32 gain) : gain(gain) {}

vector<Float32> Amplifier::amplify(vector<Float32> in) {
    Float32 scaleFactor = pow(10, gain / 20);
    vector<Float32> out;
    out.reserve(in.size());

    for (auto& sample : in) {
        out.push_back(sample * scaleFactor);
    }

    return out;
}