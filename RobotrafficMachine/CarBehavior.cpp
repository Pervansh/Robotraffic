#include <EEPROM.h>
#include <GyverPID.h>
#include <Vector.h>
#include "config.h"
#include "CarBehavior.h"
#include "Subscriber.h"
#include "CarModules.h"

CarBehavior::CarBehavior(int iP = 0, int iI = 4, int iD = 8) : indP(iP), indI(iI), indD(iD) {
    angleServo.attach(config::angleServoPin);
    speedServo.attach(config::speedServoPin);
    Wire.begin();
    leftOctoliner = new Octoliner(42);
    rightOctoliner = new Octoliner(43);
    leftOctoliner->begin();
    leftOctoliner->setSensitivity(200);
    leftOctoliner->setBrightness(255);
    rightOctoliner->begin();
    rightOctoliner->setSensitivity(200);
    rightOctoliner->setBrightness(255);
    pid = new GyverPID();
    useStandartPID();
    pid->setDirection(REVERSE);
    pid->setpoint = 0;
    pid->setLimits(-90, 90);   //Min, max servo angles
    listners = new Vector<Subscriber*>[ListnerTypeAmount];
    mods = Vector<Module*>(0);
    mods.push_back(new MoveModule(this));
    isRunning = false;
}

void CarBehavior::addModule(Module* module) {
    if (module) {
        mods.push_back(module);
    }
}

void CarBehavior::useStandartPID() {
    pid->Kp = 90;
    pid->Ki = 0;
    pid->Kd = 0;
}

void CarBehavior::safePidInEeprom() {
    
}

void CarBehavior::calibrateSpeedServo() {
    for (int pos = 90; pos <= 100; pos += 1) { 
        speedServo.write(pos);            
        delay(150);            
    }
}

void CarBehavior::notifySubscribers(CarBehavior::ListnerType type) {
    for (int i = 0; i < listners[(int)type].size(); i++) {
        listners[(int)type][i]->update(this, type);
    }
    for (int i = 0; i < mods.size(); i++) {
        mods[i]->update(this, type);
    }
}

void CarBehavior::subscribe(Subscriber* subscriber, CarBehavior::ListnerType type) {
    listners[(int)type].push_back(subscriber);
}

void CarBehavior::execute() {
    speed = 90;
    angle = 102;
    anglePriority = 0;
    speedPriority = 0;
    notifySubscribers(CarBehavior::ListnerType::onExcecute);
    if (isRunning) {
        notifySubscribers(CarBehavior::ListnerType::onRunning);
        for(int i = 0; i < mods.size(); i++) {
            mods[i]->process();
        }
    }
    angleServo.write(angle);
    speedServo.write(constrain(speed, config::fullSpeed, 120));
}

void CarBehavior::run() {
    if (!isRunning) {
        notifySubscribers(CarBehavior::ListnerType::onRun);
        isRunning = true;
    }
}

void CarBehavior::stop() {
    if (isRunning) {
        notifySubscribers(CarBehavior::ListnerType::onStop);
        isRunning = false;
    }
}

CarBehavior::~CarBehavior() {
    notifySubscribers(CarBehavior::ListnerType::onDelete);
    for (int i = 0; i < mods.size(); i++) {
        delete mods[i];
    }
}
