//
// Created by Jeremi Campagna on 2024-07-30.
//

#ifndef EQ_CPP_CONFIG_H
#define EQ_CPP_CONFIG_H

#include "../lib/json.hpp"
#include <fstream>

class Config {
public:
    Config();

    bool loadConfig();

    bool ampToggle;
    float ampGain;

    bool equalizerToggle;
    std::vector<float> equalizerF;
    std::vector<float> equalizerQ;
    std::vector<float> equalizerG;

    bool reverbToggle;
    float reverbDryWet;
    std::string irFilePath;
};


#endif //EQ_CPP_CONFIG_H
