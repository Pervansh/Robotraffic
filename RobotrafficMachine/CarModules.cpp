#include <GyverPID.h>
#include "CarModules.h"
#include "Module.h"
#include "CarBehavior.h"

MoveModule::MoveModule(CarBehavior* cb) : Module(cb) {
    octoliner = cb->getOctoliner();
    pid = cb->getPID();
}

void MoveModule::process() {
    int dataSensors[8];
    int crossCnt = 0;
    for (int i = 0; i < 8; i++) {
        dataSensors[i] = octoliner->analogRead(i);
        if(dataSensors[i] >= 1000) {
            crossCnt++;
        }
    }
    pid->input = octoliner->mapLine(dataSensors);
    int angle = pid->getResultTimer();
    
    if (crossCnt >= 5) {
        angle = constrain(angle, 80, 100);
    }
    
    getCarBehavior()->setAngle(angle);
}
