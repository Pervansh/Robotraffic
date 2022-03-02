#include <EEPROM.h>
#include <GyverPID.h>
#include <Vector.h>
#include "config.h"
#include "CarBehavior.h"
#include "Subscriber.h"
#include "CarModules.h"

CarBehavior::CarBehavior(uint16_t* _pidAddresses) : pidAddresses(_pidAddresses) {
    angleServo.attach(config::angleServoPin);
    speedServo.attach(config::speedServoPin);
    Wire.begin();
    leftOctoliner = new Octoliner(43);
    rightOctoliner = new Octoliner(42);
    leftOctoliner->begin();
    leftOctoliner->setSensitivity(200);
    leftOctoliner->setBrightness(255);
    rightOctoliner->begin();
    rightOctoliner->setSensitivity(200);
    rightOctoliner->setBrightness(255);
    enginePower = 25.75;
    pid = new GyverPID();
    pid->setDirection(REVERSE);
    pid->setpoint = 0;
    pid->setLimits(-180, 180);   //Min, max servo angles

    useStandartPID();
    
    if (pidAddresses) {
        takePidFromEeprom();
    }
    
    listners = new Vector<Subscriber*>[ListnerTypeAmount];
    mods = Vector<Module*>();
    isRunning = false;
}

void CarBehavior::addModule(Module* module) {
    if (module) {
        mods.push_back(module);
    }
}

void CarBehavior::useStandartPID() {
    pid->Kp = 55;
    pid->Ki = 4.5;
    pid->Kd = 7.5;
}

void CarBehavior::resetPID() {
    GyverPID* newPid = new GyverPID();
    newPid->Kp = pid->Kp;
    newPid->Ki = pid->Ki;
    newPid->Kd = pid->Kd;
    delete pid;
    newPid = pid;
}

void CarBehavior::safePidInEeprom() {
    if (pidAddresses) {
        EEPROM.put(pidAddresses[0], pid->Kp);
        EEPROM.put(pidAddresses[1], pid->Ki);
        EEPROM.put(pidAddresses[2], pid->Kd);
    }
}

void CarBehavior::takePidFromEeprom() {
    if (pidAddresses) {
        EEPROM.get(pidAddresses[0], pid->Kp);
        EEPROM.get(pidAddresses[1], pid->Ki);
        EEPROM.get(pidAddresses[2], pid->Kd); 
    }
}

void CarBehavior::calibrateSpeedServo() {
    for (int pos = 90; pos <= 100; pos += 1) { 
        speedServo.write(pos);            
        delay(150);            
    }
    speedServo.write(75);
    delay(500);
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
    speed = 0;
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
    speedServo.write(90 - enginePower * speed);
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
