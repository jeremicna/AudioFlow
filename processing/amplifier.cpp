//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(double gain) : gain(gain) {}

void Amplifier::process(std::vector<float> &in) {
    double scaleFactor = pow(10, gain / 20);

    for (auto& sample : in) {
        sample *= scaleFactor;
    }
}