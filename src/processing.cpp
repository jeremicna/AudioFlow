//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "processing.h"
#include <thread>


Processing::Processing(Config& config, double volume) :
    config(config),
    amplifier(*new Amplifier(config.ampToggle, config.ampGain)),
    equalizer(*new Equalizer(config.equalizerToggle, config.equalizerF, config.equalizerQ, config.equalizerG, 48000)),
    convolutionReverb(*new ConvolutionReverb(config.reverbToggle, config.irFilePath, config.reverbDryWet)),
    volume(volume) {}

Processing::Processing(Config& config, Processing* old, double volume) :
        config(config),
        amplifier(old->amplifier),
        equalizer(old->equalizer),
        convolutionReverb(old->convolutionReverb),
        volume(volume)
{
    if (old->volume != volume) {
        amplifier.setVolumeAdjustment(volume / old->volume);
    }
    if (amplifier.getToggle() != config.ampToggle) {
        amplifier.setToggle(config.ampToggle);
    }
    if (amplifier.getGain() != config.ampGain) {
        amplifier.setGain(config.ampGain);
    }

    if (equalizer.getToggle() != config.equalizerToggle) {
        equalizer.setToggle(config.equalizerToggle);
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

    if (convolutionReverb.getToggle() != config.reverbToggle) {
        convolutionReverb.setToggle(config.reverbToggle);
    }
    if (convolutionReverb.getDryWet() != config.reverbDryWet) {
        convolutionReverb.setDryWet(config.reverbDryWet);
    } else if (convolutionReverb.path != config.irFilePath) {
        std::thread ([&]() {
            swapMutex.lock();
            ConvolutionReverb newReverb = ConvolutionReverb(config.reverbToggle, config.irFilePath, config.reverbDryWet);
            swapMutex.unlock();
        }).detach();
    }
}

void Processing::process(std::vector<float>& input) {
    amplifier.process(input);
    equalizer.process(input);
    convolutionReverb.process(input);
}