//
// Created by Jeremi Campagna on 2024-08-14.
//

#include "audioProcessor.h"

AudioProcessor::AudioProcessor(bool toggle)
    : mix(Smoother(0.0, 0.0, 0)) {
    setToggle(toggle);
};

void AudioProcessor::setToggle(bool newToggle) {
    if (newToggle) {
        setMix(1.0);
    } else {
        setMix(0.0);
    }
    toggle = newToggle;
}

bool AudioProcessor::getToggle() {
    return toggle;
}

double AudioProcessor::getMix() {
    return mix.currentValueNoChange();
}

void AudioProcessor::setMix(double newMix) {
    mix = Smoother(this->mix.currentValueNoChange(), newMix, smootherSteps);
}

