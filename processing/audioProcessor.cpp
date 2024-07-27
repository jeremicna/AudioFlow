//
// Created by Jeremi Campagna on 2024-07-16.
//

#include "audioProcessor.h"


AudioProcessor::AudioProcessor(Amplifier amplifier, Equalizer equalizer, ConvolutionReverb convolutionReverb) : amplifier(amplifier), equalizer(equalizer), convolutionReverb(convolutionReverb) {}

void AudioProcessor::process(std::vector<float>& input) {
    amplifier.process(input);
    equalizer.process(input);
    convolutionReverb.process(input);
}