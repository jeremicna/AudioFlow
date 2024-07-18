//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AUDIOPROCESSOR_H
#define EQ_CPP_AUDIOPROCESSOR_H

#include <CoreAudio/CoreAudio.h>
#include <map>
#include "Amplifier.h"
#include "Equalizer.h"

using namespace std;

class AudioProcessor {
public:
    AudioProcessor(Amplifier amplifier, Equalizer equalizer);

    void process(vector<Float32>& in);

private:
    Amplifier amplifier;
    Equalizer equalizer;
};


#endif //EQ_CPP_AUDIOPROCESSOR_H
