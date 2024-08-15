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

    bool ampToggle = data.at("amplifier").at("toggle").get<bool>();
    float ampGain = data.at("amplifier").at("g").get<float>();

    bool equalizerToggle = data.at("equalizer").at("toggle").get<bool>();
    std::vector<float> equalizerF = data.at("equalizer").at("f").get<std::vector<float>>();
    std::vector<float> equalizerQ = data.at("equalizer").at("q").get<std::vector<float>>();
    std::vector<float> equalizerG = data.at("equalizer").at("g").get<std::vector<float>>();

    bool reverbToggle = data.at("reverb").at("toggle").get<bool>();
    float reverbDryWet = data.at("reverb").at("dw").get<float>();
    std::string irFilePath = data.at("reverb").at("ir").get<std::string>();

    if (ampToggle != this->ampToggle || ampGain != this->ampGain || equalizerToggle != this->equalizerToggle || equalizerF != this->equalizerF || equalizerQ != this->equalizerQ || equalizerG != this->equalizerG || reverbToggle != this->reverbToggle || reverbDryWet != this->reverbDryWet || irFilePath != this->irFilePath) {
        this->ampToggle = ampToggle;
        this->ampGain = ampGain;

        this->equalizerToggle = equalizerToggle;
        this->equalizerF = equalizerF;
        this->equalizerQ = equalizerQ;
        this->equalizerG = equalizerG;

        this->reverbToggle = reverbToggle;
        this->reverbDryWet = reverbDryWet;
        this->irFilePath = irFilePath;

        return false;
    }

    return true;
}