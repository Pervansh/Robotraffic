#pragma once

#include "CarBehavior.h"
class CarBehavior;

class Subscriber {
public:
    Subscriber() {}

    virtual void update(CarBehavior* behavior, CarBehavior::ListnerType type) = 0;
};
