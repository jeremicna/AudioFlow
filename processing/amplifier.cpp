//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(Float32 gain) : gain(gain) {}

void Amplifier::process(vector<Float32>& in) {
    Float32 scaleFactor = pow(10, gain / 20);

    for (auto& sample : in) {
        sample *= scaleFactor;
    }
}