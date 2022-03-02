#pragma once

#include "CarBehavior.h"

class EmptyBehavior : public CarBehavior {
public:
    EmptyBehavior() : CarBehavior(nullptr) {}
};
