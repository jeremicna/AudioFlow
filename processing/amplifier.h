//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>

using namespace std;

class Amplifier {
public:
    Amplifier(float gain);

    void process(vector<float>& in);
private:
    float gain;
};


#endif //EQ_CPP_AMPLIFIER_H
