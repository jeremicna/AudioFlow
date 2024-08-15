//
// Created by Jeremi Campagna on 2024-07-17.
//

#ifndef EQ_CPP_EQUALIZER_H
#define EQ_CPP_EQUALIZER_H

#include <map>
#include "iirFilter.h"
#include "audioProcessor.h"

class Equalizer: public AudioProcessor {
public:
    Equalizer(std::vector<float>& fVector, std::vector<float>& qVector, std::vector<float>& gVector, float sampleRate);

    void process(std::vector<float>& input);

    std::vector<IIRFilter>* getFilters();
private:
    std::vector<IIRFilter>* filters;
};


#endif //EQ_CPP_EQUALIZER_H
