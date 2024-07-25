//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "audioProcessor.h"

AudioProcessor::AudioProcessor(Amplifier amplifier, Equalizer equalizer) : amplifier(amplifier), equalizer(equalizer) {}

void AudioProcessor::process(vector<Float32>& input) {
    amplifier.process(input);
    equalizer.process(input);
}