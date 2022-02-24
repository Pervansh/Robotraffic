#pragma once

#include "Module.h"
#include <inttypes.h>
#include <SoftwareSerial.h>

class Module;
class CarBehavior;
class Octoliner;
class Servo;
class SoftwareSerial;

/*////////////////
    Octoliner
////////////////*/
class MoveModule : public Module {
private:
    Servo* throttle;
    Servo* rotator;
    Octoliner* leftPlank;
    Octoliner* rightPlank;
    GyverPID* pid;
    float* octolinerCoefs;
    float octolinerOffset = 7.5;

    int pos = 0;
    float rotatorMidPos = 102;
    float lastError = 0;
    unsigned long timer = 0;
public:
    MoveModule(CarBehavior* cb, float* _octolinerCoefs = nullptr);
    MoveModule(CarBehavior* cb);

    void process();
    void update(CarBehavior* behavior, CarBehavior::ListnerType type);
    float calculateError();

    void setOctolinerOffset(float newOctolinerOffset) { octolinerOffset = newOctolinerOffset; }
    float getOctolinerOffset() { return octolinerOffset; }

    ~MoveModule() { delete octolinerCoefs; }
};

/*////////////////
    Irda
////////////////*/
class IrdaModule : public Module {
private:
    SoftwareSerial stream = SoftwareSerial(0, 1);
    Octoliner* leftPlank;
    Octoliner* rightPlank;
    bool useStopLinning;
public:
    const uint8_t RX;
    const uint8_t TX;
    
    IrdaModule(CarBehavior* cb, uint8_t rx, uint8_t tx, bool _useStopLinning = false);

    void process();

    ~IrdaModule() {}
};

/*////////////////
    SharpModule
////////////////*/
class SharpModule : public Module {
private:
    bool wasStopped;
    long timer;
public:
    SharpModule(CarBehavior* cb);

    void process();

    uint16_t get_gp2d12(uint16_t value);

    ~SharpModule() {}
};
