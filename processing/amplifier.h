//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>

class Amplifier {
public:
    Amplifier(double gain);

    void process(std::vector<float>& in);
private:
    double gain;
};


#endif //EQ_CPP_AMPLIFIER_H
