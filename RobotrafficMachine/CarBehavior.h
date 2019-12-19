#pragma once
#include <Arduino.h>
#include <Servo.h>
#include <GyverPID.h>
#include <Vector.h>
#include <GpioExpander.h>
#include <i2cioCommands.h>
#include <Octoliner.h>

#include <Module.h>
#include <CarModules.h>

class CarBehavior {
private:
    Servo angleServo;
    Servo speedServo;
    GyverPID* pid;
    Vector<Module> mods;
    Octoliner* octoliner;
    bool isRunning;
    int speed;
    int angle;
public:
    CarBehavior(int angleServoPin, int speedServoPin) {
        Servo angleServo = Servo(angleServoPin);
        Servo speedServo = Servo(speedServoPin);
        octoliner = new Octoliner(42);
        pid = new GyverPID();
        useStandartPID();
        pid->setLimits(0, 180);
        mods.push_back(MoveModule(this, pid));
        isRunning = false;
    }

    virtual void execute() {
        speed = 1000;
        angle = 90;

        for(Module mod : mods) {
            mod.process();
        }

        angleServo.write(angle);
        speedServo.write(speed);
    }
    virtual void run() = 0;
    virtual void stop() = 0;
    virtual void useStandartPID() = 0;

    GyverPID* getPID() {return pid;}
    void setPID(GyverPID* pid) {this->pid = pid;}
    Octoliner* getOctoliner() {return octoliner;}
    int getSpeed() {return speed;}
    int getAngle() {return angle;}
    virtual void setSpeed(int speed) {this->speed = speed;}
    virtual void setAngle(int angle) {this->angle = angle;}
};