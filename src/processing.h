//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_PROCESSING_H
#define EQ_CPP_PROCESSING_H

#include <map>
#include "../processing/audioProcessor.h"
#include "../processing/amplifier.h"
#include "../processing/equalizer.h"
#include "../processing/convolutionReverb.h"
#include "../fileutils/config.h"
#include "../fileutils/readIRFile.h"

class Processing {
public:
    Processing(Config& config);
    Processing(Config& config, Processing* old);

    void process(std::vector<float>& input);
private:
    Config config;
    Amplifier& amplifier;
    Equalizer& equalizer;
    ConvolutionReverb& convolutionReverb;
};


#endif //EQ_CPP_PROCESSING_H
