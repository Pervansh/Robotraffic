#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <GyverPID.h>
#include <Vector.h>
#include <GpioExpander.h>
#include <i2cioCommands.h>
#include <Octoliner.h>

#include "Module.h"
#include "CarModules.h"

class Module;
class MoveModule;

class CarBehavior {
private:
    Servo angleServo;
    Servo speedServo;
    GyverPID* pid;
    Vector<Module*> mods;
    Octoliner* octoliner;
    bool isRunning;
    int speed;
    int angle;
public:
    CarBehavior(int angleServoPin, int speedServoPin);

    virtual void execute();
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void useStandartPID() {}

    void addModule(Module* module);
    GyverPID* getPID() {return pid;}
    void setPID(GyverPID* pid) {this->pid = pid;}
    Octoliner* getOctoliner() {return octoliner;}
    int getSpeed() {return speed;}
    int getAngle() {return angle;}
    virtual void setSpeed(int speed) {this->speed = speed;}
    virtual void setAngle(int angle) {this->angle = angle;}

    ~CarBehavior();
};
