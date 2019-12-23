#pragma once

#include "Module.h"
//#include "CarBehavior.h"

class Module;
class CarBehavior;

class MoveModule : public Module {
private:
    Octoliner* octoliner;
    GyverPID* pid;
public:
    MoveModule(CarBehavior* cb);

    void process();

    ~MoveModule() {}
};
