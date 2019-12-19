#pragma once

#include <CarBehavior.h>

class Module {
private:
    CarBehavior* behavior;
    const int priority;
public:
    Module(CarBehavior* behavior, int prior = 64) : priority(prior) {
        this->behavior = behavior;
        this->priority;
    }

    virtual void process() = 0;

    int getPriority() {
        return priority;
    }

    ~Module() {}
};
