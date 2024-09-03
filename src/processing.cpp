//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "processing.h"
#include <thread>
#include <mutex>
#include "iostream"


Processing::Processing(const Config& config, double volume) :
    config(config),
    amplifier(std::make_shared<Amplifier>(config.ampToggle, config.ampGain)),
    equalizer(std::make_shared<Equalizer>(config.equalizerToggle, config.equalizerF, config.equalizerQ, config.equalizerG, 48000)),
    convolutionReverb(std::make_shared<ConvolutionReverb>(config.reverbToggle, config.irFilePath, config.reverbDryWet)),
    volume(volume) {}

Processing::Processing(const Config& config, const Processing* old, double volume) :
        config(config),
        amplifier(std::move(old->amplifier)),
        equalizer(std::move(old->equalizer)),
        convolutionReverb(std::move(old->convolutionReverb)),
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
        std::thread ([this, config]() {
            convolutionReverb->setToggle(false);
            auto newConvolutionReverb = std::make_shared<ConvolutionReverb>(config.reverbToggle, config.irFilePath, config.reverbDryWet);

            std::lock_guard<std::mutex> lock(swapMutex);
            convolutionReverb = std::move(newConvolutionReverb);
        }).detach();
    }
}

void Processing::process(std::vector<float>& input) {
    std::vector<double> doubleInput(input.begin(), input.end());

    amplifier->process(doubleInput);
    equalizer->process(doubleInput);

    std::vector<float> processed(doubleInput.begin(), doubleInput.end());
    input = processed;

    std::lock_guard<std::mutex> lock(swapMutex);
    convolutionReverb->process(input);
}