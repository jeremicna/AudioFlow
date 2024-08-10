//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "audioProcessor.h"
#include <chrono>

AudioProcessor::AudioProcessor(Config& config) :
    config(config),
    amplifier(*new Amplifier(config.ampGain)),
    equalizer(*new Equalizer(config.equalizerF, config.equalizerQ, config.equalizerG, 48000)),
    convolutionReverb(*new ConvolutionReverb(config.irFilePath, config.reverbDryWet))
{}

AudioProcessor::AudioProcessor(Config& config, AudioProcessor* old) :
        config(config),
        amplifier(old->amplifier),
        equalizer(old->equalizer),
        convolutionReverb(old->convolutionReverb)
{
    if (amplifier.getGain() != config.ampGain) {
        amplifier.setGain(config.ampGain);
    }

    std::vector<IIRFilter>& filters = *equalizer.getFilters();
    for (size_t i = 0; i < filters.size(); ++i) {
        if (filters[i].getF() != config.equalizerF[i]) {
            filters[i].setF(config.equalizerF[i]);
        }
        if (filters[i].getQ() != config.equalizerQ[i]) {
            filters[i].setQ(config.equalizerQ[i]);
        }
        if (filters[i].getG() != config.equalizerG[i]) {
            filters[i].setG(config.equalizerG[i]);
        }
    }

    if (convolutionReverb.getDryWet() != config.reverbDryWet) {
        convolutionReverb.setDryWet(config.reverbDryWet);
    } else if (convolutionReverb.path != config.irFilePath) {
        convolutionReverb = ConvolutionReverb(config.irFilePath, config.reverbDryWet);
    }
}

void AudioProcessor::process(std::vector<float>& input) {
    amplifier.process(input);
    equalizer.process(input);
    convolutionReverb.process(input);
}