//
// Created by Jeremi Campagna on 2024-07-30.
//

#include "config.h"
#include "iostream"

using json = nlohmann::json;

Config::Config() {
    loadConfig();
}

bool Config::loadConfig() {
    std::ifstream f("../config.json");
    json data = json::parse(f);

    float ampGain = data.at("amplifier").at("g").get<float>();

    std::vector<float> equalizerF = data.at("equalizer").at("f").get<std::vector<float>>();
    std::vector<float> equalizerQ = data.at("equalizer").at("q").get<std::vector<float>>();
    std::vector<float> equalizerG = data.at("equalizer").at("g").get<std::vector<float>>();

    float reverbDryWet = data.at("reverb").at("dw").get<float>();
    std::string irFilePath = data.at("reverb").at("ir").get<std::string>();

    if (ampGain != this->ampGain || equalizerF != this->equalizerF || equalizerQ != this->equalizerQ || equalizerG != this->equalizerG || reverbDryWet != this->reverbDryWet || irFilePath != this->irFilePath) {
        this->ampGain = ampGain;

        this->equalizerF = equalizerF;
        this->equalizerQ = equalizerQ;
        this->equalizerG = equalizerG;

        this->reverbDryWet = reverbDryWet;
        this->irFilePath = irFilePath;

        return false;
    }

    return true;
}