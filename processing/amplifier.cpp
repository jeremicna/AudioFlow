//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(float gain) : gain(Smoother(gain, gain, 0)) {}

void Amplifier::process(std::vector<float> &input) {
    for (auto& sample : input) {
        double scaleFactor = pow(10, gain.currentValue() / 20);
        double dw = mix.currentValue();
        double scaled = sample * scaleFactor;
        sample = scaled * dw + sample * (1 - dw);
    }
}

float Amplifier::getGain() {
    return gain.currentValueNoChange();
}

void Amplifier::setGain(float gain) {
    this->gain = Smoother(this->gain.currentValueNoChange(), gain, 256);
}