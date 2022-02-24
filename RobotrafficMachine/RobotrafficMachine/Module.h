#pragma once

#include "CarBehavior.h"
#include "Subscriber.h"

class CarBehavior;

class Module : Subscriber {
private:
    CarBehavior* behavior;
public:
    const int priority;
    
    Module(CarBehavior* behavior, int prior = 64) : priority(prior) {
        this->behavior = behavior;
    }

    virtual void process() = 0;

    virtual void update(CarBehavior* behavior, CarBehavior::ListnerType type){}

    CarBehavior* getCarBehavior() {return behavior;}

    virtual ~Module() = default;
};
