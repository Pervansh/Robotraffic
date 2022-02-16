#include "Arduino.h"
#include "CarModules.h"
#include "Module.h"
#include "CarBehavior.h"
#include <GyverPID.h>
#include "config.h"

#define R1 2  // 1
#define R2 3  // 2

MoveModule::MoveModule(CarBehavior* cb, float* _octolinerCoefs) : Module(cb) {
  if (_octolinerCoefs) {
    octolinerCoefs = _octolinerCoefs;
  } else {
    octolinerCoefs = new float[16]; // Утечка данных
    for (int i = 0; i < 16; ++i) {
      octolinerCoefs[i] = 1.0;
    }
  }

  leftPlank = cb->getLeftOctoliner();
  rightPlank = cb->getRightOctoliner();
  rotatorMidPos = cb->midAngle;
  pid = cb->getPID();
}

void MoveModule::process() {
  float error = calculateError();
  pid->input = error;
  pid->setDt(millis() - timer);
  float angle = rotatorMidPos + pid->getResult();
  getCarBehavior()->changeAngle(angle, 10);
  timer = millis();
  getCarBehavior()->changeSpeed(1, 10);
}

float MoveModule::calculateError() {
  float dataSensors[16];
  float totalS = 0;
  float sum = 0;
  long checkSet = 0;
  for (int i = 0; i < 8; i++) {
    dataSensors[i] = leftPlank->analogRead(7 - i);
    if (dataSensors[i] >= 250) {
      checkSet += 1 << (i + 1);
      sum += octolinerCoefs[i] * (i - octolinerOffset) * dataSensors[i];
      totalS += dataSensors[i];
    }
  }
  for (int i = 8; i < 15; i++) {
    dataSensors[i] = rightPlank->analogRead(15 - i);
    if (dataSensors[i] >= 250) {
      checkSet += 1 << (i + 1);
      sum += octolinerCoefs[i] * (i - octolinerOffset) * dataSensors[i];
      totalS += dataSensors[i];
    }
  }

  if (totalS) {
    float error = ((double)sum / (double)totalS / 7);
    lastError = error;
    return error;
  } else if (checkSet == 1) {
    return -octolinerCoefs[0];
  } else if (checkSet == 1 << 15) {
    return octolinerCoefs[14];
  } else {
    return lastError;
  }
}

void MoveModule::update(CarBehavior* behavior, CarBehavior::ListnerType type) {
  if (type == CarBehavior::ListnerType::onRun) {
    timer = millis();
  }
}

IrdaModule::IrdaModule(CarBehavior* cb, uint8_t rx, uint8_t tx, bool _useStopLinning) : Module(cb), RX(rx), TX(tx) {
  useStopLinning = _useStopLinning;
  stream = SoftwareSerial(rx, tx);
  stream.begin(115200);
  leftPlank = cb->getLeftOctoliner();
  rightPlank = cb->getRightOctoliner();
}

void IrdaModule::process() {
  if (stream.available() == 0) {
    return;
  }

  // getCarBehavior()->changeAngle(102, 20);
  getCarBehavior()->changeSpeed(0.925, 20);

  int type = stream.read();
  Serial.println(type);
  stream.flush();

  bool l1 = leftPlank->analogRead(7) >= 250 && false;
  bool r2 = rightPlank->analogRead(7 - R2) >= 250;
  bool r1 = rightPlank->analogRead(7 - R1) >= 250;

  bool lineCond = (!l1 && r2 && r1) || !useStopLinning;

  if ((type == 1 || type == 0) && lineCond) {
    getCarBehavior()->changeSpeed(0, 20);
  } else if (type == 3 || type == 4 || type == 0) {
    getCarBehavior()->changeSpeed(0.8, 20);
  }
}

SharpModule::SharpModule(CarBehavior* cb) : Module(cb) {
  pinMode(config::sharpPin, INPUT);
  wasStopped = false;
  timer = 0;
}

void SharpModule::process() {
  uint16_t range = get_gp2d12(analogRead(config::sharpPin));
  range = min(range, config::sharpReductionRange);
  range = max(range, config::sharpBreakRange);
  float speed = (float)(range - config::sharpBreakRange) / (config::sharpReductionRange - config::sharpBreakRange);
  getCarBehavior()->changeSpeed(speed, 15);
}

uint16_t SharpModule::get_gp2d12(uint16_t value)  {
  if (value < 10) value = 10;
  uint16_t range = ((67870.0 / (value - 3.0)) - 40.0);
  return range * 10 / 7;
}
