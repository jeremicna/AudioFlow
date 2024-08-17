//
// Created by Jeremi Campagna on 2024-08-02.
//

#ifndef EQ_CPP_SMOOTHER_H
#define EQ_CPP_SMOOTHER_H


class Smoother {
public:
    Smoother(double start, double target, double steps);

    double currentValue();
    double currentValueNoChange();

    int getRemaining();
private:
    double target;
    double current;
    double increment;
    int remaining;
};


#endif //EQ_CPP_SMOOTHER_H
