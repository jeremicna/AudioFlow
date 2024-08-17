//
// Created by Jeremi Campagna on 2024-08-14.
//

#ifndef EQ_CPP_AUDIOPROCESSOR_H
#define EQ_CPP_AUDIOPROCESSOR_H

#include <map>
#include "smoother.h"

class AudioProcessor {
public:
    AudioProcessor(bool toggle);

    void process(std::vector<float>& input);

    double getMix();
    void setMix(double mix);

    void setToggle(bool toggle);
    bool getToggle();
protected:
    Smoother mix;

private:
    bool toggle;
};


#endif //EQ_CPP_AUDIOPROCESSOR_H
