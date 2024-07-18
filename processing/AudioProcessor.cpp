//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "AudioProcessor.h"

AudioProcessor::AudioProcessor(Amplifier amplifier, Equalizer equalizer) : amplifier(amplifier), equalizer(equalizer) {}

void AudioProcessor::process(vector<Float32>& in) {
    amplifier.amplify(in);
    equalizer.process(in);
}