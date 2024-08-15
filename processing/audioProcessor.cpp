//
// Created by Jeremi Campagna on 2024-08-14.
//

#include "audioProcessor.h"

AudioProcessor::AudioProcessor() : dryWet(Smoother(1, 1, 0)), toggle(true) {
};

void AudioProcessor::setToggle(bool toggle) {
    if (toggle) {
        setDryWet(1);
    } else {
        setDryWet(0);
    }
    this->toggle = toggle;
}

bool AudioProcessor::getToggle() {
    return toggle;
}

double AudioProcessor::getDryWet() {
    return dryWet.currentValueNoChange();
}

void AudioProcessor::setDryWet(double dryWet) {
    this->dryWet = Smoother(this->dryWet.currentValueNoChange(), dryWet, 256);
}

