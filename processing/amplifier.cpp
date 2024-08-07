//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(float gain) : gain(Smoother(gain, gain, 0)) {}

void Amplifier::process(std::vector<float> &in) {
    for (auto& sample : in) {
        double scaleFactor = pow(10, gain.currentValue() / 20);
        sample *= scaleFactor;
    }
}

float Amplifier::getGain() {
    return gain.currentValueNoChange();
}

void Amplifier::setGain(float gain) {
    this->gain = Smoother(this->gain.currentValueNoChange(), gain, 256);
}