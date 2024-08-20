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
#include "../processing/smoother.h"

class Processing {
public:
    Processing(const Config& config, double volume);
    Processing(const Config& config, const Processing* old, double volume);

    void process(std::vector<float>& input);
private:
    double volume;
    Config config;
    std::shared_ptr<Amplifier> amplifier;
    std::shared_ptr<Equalizer> equalizer;
    std::shared_ptr<ConvolutionReverb> convolutionReverb;
    std::mutex swapMutex;
};


#endif //EQ_CPP_PROCESSING_H
