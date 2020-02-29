#pragma once

#include "Subscriber.h"

class CarStatistics : public Subscriber {
private:
    static int avarageLineError;
public:
    CarStatistics() {}

    void update(CarBehavior* behavior, CarBehavior::ListnerType type);
};
