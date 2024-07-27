//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(float gain) : gain(gain) {}

void Amplifier::process(vector<float>& in) {
    float scaleFactor = pow(10, gain / 20);

    for (auto& sample : in) {
        sample *= scaleFactor;
    }
}