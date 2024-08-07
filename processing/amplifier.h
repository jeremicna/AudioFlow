//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>
#include "smoother.h"

class Amplifier {
public:
    Amplifier(float gain);

    void process(std::vector<float>& in);

    float getGain();
    void setGain(float gain);
private:
    Smoother gain;
};


#endif //EQ_CPP_AMPLIFIER_H
