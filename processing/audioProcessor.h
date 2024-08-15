//
// Created by Jeremi Campagna on 2024-08-14.
//

#ifndef EQ_CPP_AUDIOPROCESSOR_H
#define EQ_CPP_AUDIOPROCESSOR_H

#include <map>
#include "smoother.h"

class AudioProcessor {
public:
    AudioProcessor();

    void process(std::vector<float>& input);

    double getDryWet();
    void setDryWet(double dryWet);

    void setToggle(bool toggle);
    bool getToggle();
protected:
    Smoother dryWet;

private:
    bool toggle;
};


#endif //EQ_CPP_AUDIOPROCESSOR_H
