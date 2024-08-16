//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>
#include "smoother.h"
#include "audioProcessor.h"

class Amplifier: public AudioProcessor {
public:
    Amplifier(float gain);

    void process(std::vector<float>& input);

    float getGain();
    void setGain(float gain);

    float getVolumeAdjustment();
    void setVolumeAdjustment(float volumeAdjustment);
private:
    Smoother gain;
    Smoother volumeAdjustment;
};


#endif //EQ_CPP_AMPLIFIER_H
