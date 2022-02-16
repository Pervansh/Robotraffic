#pragma once
#include <Arduino.h>
#include <inttypes.h>
#include <Servo.h>
#include <GyverPID.h>
#include <Vector.h>
#include <GpioExpander.h>
#include <i2cioCommands.h>
#include <Octoliner.h>
#include "config.h"

class Module;
class MoveModule;
class Subscriber;

class CarBehavior {
public:
    enum class ListnerType : uint8_t {
        onRun,
        onStop,
        onExcecute,
        onRunning,
        onDelete
    };

    const uint8_t ListnerTypeAmount = 5;

private:
    Servo angleServo;
    Servo speedServo;
    GyverPID* pid;
    Vector<Module*> mods;
    Vector<Subscriber*>* listners;
    Octoliner* leftOctoliner;
    Octoliner* rightOctoliner;
    bool isRunning;
    float speed;
    float enginePower;
    uint8_t speedPriority;
    float angle;
    uint8_t anglePriority;
public:
    const int indP, indI, indD;
    const float midAngle = 102;
    
    CarBehavior(int iP, int iI, int iD);

    virtual void execute();
    virtual void run();
    virtual void stop();
    virtual void useStandartPID();
    virtual void resetPID();
    void calibrateSpeedServo();

    void addModule(Module* module);
    void safePidInEeprom();         // Doesn't work
    void notifySubscribers(ListnerType);
    void subscribe(Subscriber*, ListnerType);
    
    GyverPID* getPID() {return pid;}
    void setPID(GyverPID* pid) {this->pid = pid;}
    Octoliner* getLeftOctoliner() {return leftOctoliner;}
    Octoliner* getRightOctoliner() {return rightOctoliner;}
    bool wasRunning() {return isRunning;}
    float getSpeed() {return speed;}
    float getEnginePower() {return enginePower;}
    float getAngle() {return angle;}
    virtual void changeSpeed(float speed, uint8_t priority) {
        if (priority >= speedPriority){
            this->speed = speed;
        }
    }
    virtual void setEnginePower(float power) {enginePower = power;}
    virtual void changeAngle(float angle, uint8_t priority) {
        if (priority >= anglePriority){
            this->angle = angle;
        }
    }

    virtual ~CarBehavior();
};
