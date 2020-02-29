#include "Arduino.h"
#include "CarModules.h"
#include "Module.h"
#include "CarBehavior.h"
#include <GyverPID.h>
#include "config.h"

MoveModule::MoveModule(CarBehavior* cb) : Module(cb) {
    leftPlank = cb->getLeftOctoliner();
    rightPlank = cb->getRightOctoliner();
    rotatorMidPos = cb->midAngle;
    pid = cb->getPID();
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void MoveModule::process() {
    float error = calculateError();
    pid->input = error;
    pid->setDt(millis() - timer);
    float angle = rotatorMidPos + pid->getResult();
    getCarBehavior()->changeAngle(angle, 10);
    timer = millis();
    getCarBehavior()->changeSpeed(80.0, 10);
}

float MoveModule::calculateError() { 
    long dataSensors[16];
    long totalS = 0;
    long sum = 0;
    for (int i = 0; i < 8; i++) {
        dataSensors[i] = leftPlank->analogRead(7 - i);
        if (dataSensors[i] >= 250) {
            sum += (i) * dataSensors[i];
            totalS += dataSensors[i];
        }
    }
    for (int i = 8; i < 15; i++) {
        dataSensors[i] = rightPlank->analogRead(15 - i);
        if (dataSensors[i] >= 250) {
            sum += (i) * dataSensors[i];
            totalS += dataSensors[i];
        }
    }
    if(totalS) {
        float error = ((double)sum / (double)totalS / 7) - 1;
        lastError = error;
        return error;
    } else {
        return lastError;
    }
}

void MoveModule::update(CarBehavior* behavior, CarBehavior::ListnerType type) {
    if (type == CarBehavior::ListnerType::onRun) {
        timer = millis();
    }
}

IrdaModule::IrdaModule(CarBehavior* cb, uint8_t rx, uint8_t tx) : Module(cb), RX(rx), TX(tx) {
    stream = SoftwareSerial(rx, tx);
}

void IrdaModule::process() {

}

SharpModule::SharpModule(CarBehavior* cb) : Module(cb) {
    pinMode(config::sharpPin, INPUT);
    wasStopped = false;
    timer = 0;
}

void SharpModule::process() {
    uint16_t range = get_gp2d12(analogRead(config::sharpPin));

    if (range <= 170) {
        if (!wasStopped) {
            timer = millis();
            wasStopped = true;
        }
        if (millis() - timer <= 200) {
            getCarBehavior()->changeSpeed(120, 200);
        } else {
            getCarBehavior()->changeSpeed(90, 200);
        }
    } else if (wasStopped){
        wasStopped = false;
    }
}

uint16_t SharpModule::get_gp2d12(uint16_t value)  {
    if (value < 10) value = 10;
    uint16_t range = ((67870.0 / (value - 3.0)) - 40.0);
    return range * 10 / 7;
}
