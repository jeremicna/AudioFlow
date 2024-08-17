//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"
#include "iostream"

Amplifier::Amplifier(bool toggle, float gain) : AudioProcessor(toggle), gain(Smoother(gain, gain, 0)), volumeAdjustment(Smoother(1, 1, 256)) {}

void Amplifier::process(std::vector<float> &input) {
    if (mix.currentValueNoChange() > 0 || mix.getRemaining() > 0) {
        for (auto &sample: input) {
            double scaleFactor = pow(10, gain.currentValue() / 20);
            double dw = mix.currentValue();
            double scaled = sample * scaleFactor * volumeAdjustment.currentValue();
            sample = scaled * dw + sample * (1 - dw);
        }
    }
}

float Amplifier::getGain() {
    return gain.currentValueNoChange();
}

void Amplifier::setGain(float gain) {
    this->gain = Smoother(this->gain.currentValueNoChange(), gain, 256);
}

float Amplifier::getVolumeAdjustment() {
    return volumeAdjustment.currentValueNoChange();
}

void Amplifier::setVolumeAdjustment(float volumeAdjustment) {
    this->volumeAdjustment = Smoother(volumeAdjustment, 1, 8192);
}