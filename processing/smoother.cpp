//
// Created by Jeremi Campagna on 2024-08-02.
//

#include "smoother.h"

Smoother::Smoother(double start, double target, double steps) : target(target) {
    current = start;
    remaining = steps;
    increment = (target - start) / steps;
}

double Smoother::currentValueNoChange() {
    return current;
}

double Smoother::currentValue() {
    if (remaining > 0) {
        current += increment;
        remaining--;
    } else if (remaining == 0 && current != target) {
        current = target;
    }

    return current;
}

int Smoother::getRemaining() {
    return remaining;
}