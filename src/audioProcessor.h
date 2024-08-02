//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AUDIOPROCESSOR_H
#define EQ_CPP_AUDIOPROCESSOR_H

#include <map>
#include "../processing/amplifier.h"
#include "../processing/equalizer.h"
#include "../processing/convolutionReverb.h"
#include "../fileutils/config.h"
#include "../fileutils/readIRFile.h"

class AudioProcessor {
public:
    AudioProcessor(Config& config);
    AudioProcessor(Config& config, AudioProcessor* old);

    void process(std::vector<float>& input);
private:
    Config config;
    Amplifier& amplifier;
    Equalizer& equalizer;
    ConvolutionReverb& convolutionReverb;
};


#endif //EQ_CPP_AUDIOPROCESSOR_H
