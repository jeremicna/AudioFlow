//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>
#include "smoother.h"
#include "audioProcessor.h"
#include "../fileutils/globals.h"

class Amplifier: public AudioProcessor {
public:
    Amplifier(bool toggle, float gain);

    void process(std::vector<float>& input);

    float getGain();
    void setGain(float newGain);

    float getVolumeAdjustment();
    void setVolumeAdjustment(float newVolumeAdjustment);
private:
    Smoother gain;
    Smoother volumeAdjustment;
};


#endif //EQ_CPP_AMPLIFIER_H
