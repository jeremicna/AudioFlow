//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "processing.h"
#include <thread>


Processing::Processing(const Config& config, double volume) :
    config(config),
    amplifier(std::make_unique<Amplifier>(config.ampToggle, config.ampGain)),
    equalizer(std::make_unique<Equalizer>(config.equalizerToggle, config.equalizerF, config.equalizerQ, config.equalizerG, 48000)),
    convolutionReverb(std::make_unique<ConvolutionReverb>(config.reverbToggle, config.irFilePath, config.reverbDryWet)),
    volume(volume) {}

Processing::Processing(const Config& config, const Processing* old, double volume) :
        config(config),
        amplifier(std::make_unique<Amplifier>(*old->amplifier)),
        equalizer(std::make_unique<Equalizer>(*old->equalizer)),
        convolutionReverb(std::make_unique<ConvolutionReverb>(*old->convolutionReverb)),
        volume(volume)
{
    if (old->volume != volume) {
        amplifier->setVolumeAdjustment(volume / old->volume);
    }
    if (amplifier->getToggle() != config.ampToggle) {
        amplifier->setToggle(config.ampToggle);
    }
    if (amplifier->getGain() != config.ampGain) {
        amplifier->setGain(config.ampGain);
    }

    if (equalizer->getToggle() != config.equalizerToggle) {
        equalizer->setToggle(config.equalizerToggle);
    }
    std::vector<IIRFilter>& filters = equalizer->getFilters();
    for (size_t i = 0; i < filters.size(); ++i) {
        if (filters.at(i).getFrequency() != config.equalizerF.at(i)) {
            filters.at(i).setFrequency(config.equalizerF.at(i));
        }
        if (filters.at(i).getQuality() != config.equalizerQ.at(i)) {
            filters.at(i).setQuality(config.equalizerQ.at(i));
        }
        if (filters.at(i).getGain() != config.equalizerG.at(i)) {
            filters.at(i).setGain(config.equalizerG.at(i));
        }
    }

    if (convolutionReverb->getToggle() != config.reverbToggle) {
        convolutionReverb->setToggle(config.reverbToggle);
    }
    if (convolutionReverb->getDryWet() != config.reverbDryWet) {
        convolutionReverb->setDryWet(config.reverbDryWet);
    } else if (convolutionReverb->path != config.irFilePath) {
        std::thread ([&]() {
            swapMutex.lock();
            ConvolutionReverb newReverb = ConvolutionReverb(config.reverbToggle, config.irFilePath, config.reverbDryWet);
            swapMutex.unlock();
        }).detach();
    }
}

void Processing::process(std::vector<float>& input) {
    amplifier->process(input);
    equalizer->process(input);
    convolutionReverb->process(input);
}