//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "audioProcessor.h"


AudioProcessor::AudioProcessor(Config& config) :
    config(config),
    amplifier(config.ampGain),
    equalizer(config.equalizerF, config.equalizerQ, config.equalizerG, 48000),
    convolutionReverb(readIRFile(config.irFilePath), config.reverbDryWet)
{}

void AudioProcessor::process(std::vector<float>& input) {
    amplifier.process(input);
    equalizer.process(input);
    convolutionReverb.process(input);
}