//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "amplifier.h"

Amplifier::Amplifier(bool toggle, float gain) : AudioProcessor(toggle), gain(Smoother(gain, gain, 0)), volumeAdjustment(Smoother(1.0, 1.0, smootherSteps)) {}

void Amplifier::process(std::vector<float> &input) {
    double currentMix = mix.currentValueNoChange();
    double mixRemaining = mix.getRemaining();

    if (currentMix > 0 || mixRemaining > 0) {
        for (auto &sample: input) {
            double scaleFactor = std::pow(10, gain.currentValue() / 20);
            double currentMix = mix.currentValue();
            double scaled = sample * scaleFactor * volumeAdjustment.currentValue();
            sample = scaled * currentMix + sample * (1.0 - currentMix);
        }
    }
}

float Amplifier::getGain() {
    return gain.currentValueNoChange();
}

void Amplifier::setGain(float newGain) {
    gain = Smoother(gain.currentValueNoChange(), newGain, smootherSteps);
}

float Amplifier::getVolumeAdjustment() {
    return volumeAdjustment.currentValueNoChange();
}

void Amplifier::setVolumeAdjustment(float newVolumeAdjustment) {
    volumeAdjustment = Smoother(newVolumeAdjustment, 1.0, volumeSmootherSteps);
}