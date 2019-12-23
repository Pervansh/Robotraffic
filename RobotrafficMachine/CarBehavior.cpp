#include <GyverPID.h>
#include "CarBehavior.h"

CarBehavior::CarBehavior(int angleServoPin, int speedServoPin) {
    angleServo.attach(angleServoPin);
    speedServo.attach(speedServoPin);
    octoliner = new Octoliner(42);
    pid = new GyverPID();
    useStandartPID();
    pid->setLimits(0, 180);   //Min, max servo angles
    mods.push_back(new MoveModule(this));
    isRunning = false;
}

void CarBehavior::addModule(Module* module) {
    if (module) {
        mods.push_back(module);
    }
}

void CarBehavior::execute() {
    /*
    speed = 1000;
    angle = 90;
    
    for(int i = 0; i < mods.size(); i++) {
        mods[i]->process();
    }
    
    angleServo.write(angle);
    speedServo.write(speed);
    */
}

CarBehavior::~CarBehavior() {
    for (int i = 0; i < mods.size(); i++) {
        delete mods[i];
    }
}
