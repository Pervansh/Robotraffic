#pragma once

#include "CarBehavior.h"

class CarBehavior;

class Module {
private:
    CarBehavior* behavior;
public:
    const int priority;
    
    Module(CarBehavior* behavior, int prior = 64) : priority(prior) {
        this->behavior = behavior;
    }

    virtual void process() = 0;

    CarBehavior* getCarBehavior() {return behavior;}

    ~Module() {}
};
