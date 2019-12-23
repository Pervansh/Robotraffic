#pragma once

#include "CarBehavior.h"

class EmptyBehavior : public CarBehavior {
public:
    EmptyBehavior(int angleServoPin, int speedServoPin)
    : CarBehavior(angleServoPin, speedServoPin) {}
    
    void run() {}
    void stop() {}
};
