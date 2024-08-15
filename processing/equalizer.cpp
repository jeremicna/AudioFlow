//
// Created by Jeremi Campagna on 2024-07-17.
//

#include "equalizer.h"
#include "iostream"

Equalizer::Equalizer(std::vector<float> &fVector, std::vector<float> &qVector, std::vector<float> &gVector, float sampleRate) {
    if (fVector.size() != qVector.size() || fVector.size() != gVector.size()) {
        throw std::invalid_argument("Equalizer vectors must be of the same size.");
    }

    filters = new std::vector<IIRFilter>;
    for (int i = 0; i < fVector.size(); i++) {
        filters->push_back(IIRFilter(fVector[i], qVector[i], gVector[i], sampleRate));
    }
}

void Equalizer::process(std::vector<float>& input) {
    std::vector<float> processed(input);
    for (auto& filter : *filters) {
        filter.process(processed);
    }
    for (size_t i = 0; i < input.size(); ++i) {
        double dw = dryWet.currentValue();
        input[i] = (processed[i] * dw) + (input[i] * (1 - dw));
    }
}

std::vector<IIRFilter>* Equalizer::getFilters() {
    return filters;
}