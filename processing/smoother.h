//
// Created by Jeremi Campagna on 2024-08-02.
//

#ifndef EQ_CPP_SMOOTHER_H
#define EQ_CPP_SMOOTHER_H


class Smoother {
public:
    Smoother(float start, float target, float steps);

    float currentValue();
    float currentValueNoChange();

    int getRemaining();
private:
    float current;
    float increment;
    int remaining;
};


#endif //EQ_CPP_SMOOTHER_H
