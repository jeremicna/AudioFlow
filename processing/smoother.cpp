//
// Created by Jeremi Campagna on 2024-08-02.
//

#include "smoother.h"

Smoother::Smoother(float start, float target, float steps) {
    current = start;
    remaining = steps;
    increment = (target - start) / steps;
}

float Smoother::currentValueNoChange() {
    return current;
}

float Smoother::currentValue() {
    if (remaining > 0) {
        current += increment;
        remaining--;
    }

    return current;
}