//
// Created by Jeremi Campagna on 2024-07-16.
//

#ifndef EQ_CPP_AMPLIFIER_H
#define EQ_CPP_AMPLIFIER_H

#include <map>
#include <CoreAudio/CoreAudio.h>

using namespace std;

class Amplifier {
public:
    Amplifier(Float32 gain);

    void amplify(vector<Float32>& in);
private:
    Float32 gain;
};


#endif //EQ_CPP_AMPLIFIER_H
