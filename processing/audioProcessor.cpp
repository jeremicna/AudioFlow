//
// Created by Jeremi Campagna on 2024-08-14.
//

#include "audioProcessor.h"

// MAKE IT SO INIT TOGGLE AND DRY WET CORRESPONDS TO CONFIG
AudioProcessor::AudioProcessor(bool toggle) : mix(Smoother(0, 0, 0)) {
    setToggle(toggle);
};

void AudioProcessor::setToggle(bool toggle) {
    if (toggle) {
        setMix(1);
    } else {
        setMix(0);
    }
    this->toggle = toggle;
}

bool AudioProcessor::getToggle() {
    return toggle;
}

double AudioProcessor::getMix() {
    return mix.currentValueNoChange();
}

void AudioProcessor::setMix(double mix) {
    this->mix = Smoother(this->mix.currentValueNoChange(), mix, 256);
}

