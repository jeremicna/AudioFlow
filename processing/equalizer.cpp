//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "equalizer.h"
#include "iostream"

Equalizer::Equalizer(bool toggle, std::vector<float> &fVector, std::vector<float> &qVector, std::vector<float> &gVector, float sampleRate) : AudioProcessor(toggle) {
    if (fVector.size() != qVector.size() || fVector.size() != gVector.size()) {
        throw std::invalid_argument("Equalizer vectors must be of the same size.");
    }

    filters = new std::vector<IIRFilter>;
    for (int i = 0; i < fVector.size(); i++) {
        filters->push_back(IIRFilter(fVector[i], qVector[i], gVector[i], sampleRate));
    }
}

void Equalizer::process(std::vector<float>& input) {
    if (mix.currentValueNoChange() > 0 || mix.getRemaining() > 0) {
        std::vector<float> processed(input);
        for (auto &filter: *filters) {
            filter.process(processed);
        }
        for (size_t i = 0; i < input.size(); ++i) {
            double dw = mix.currentValue();
            input[i] = (processed[i] * dw) + (input[i] * (1 - dw));
        }
    }
}

std::vector<IIRFilter>* Equalizer::getFilters() {
    return filters;
}