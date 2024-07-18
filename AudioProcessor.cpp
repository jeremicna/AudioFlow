//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "AudioProcessor.h"

AudioProcessor::AudioProcessor(Amplifier amplifier, Equalizer equalizer) : amplifier(amplifier), equalizer(equalizer) {}

vector<Float32> AudioProcessor::process(vector<Float32> in) {
    vector<Float32> amplified = amplifier.amplify(in);
    vector<Float32> filtered = equalizer.process(amplified);

    return filtered;
}