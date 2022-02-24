#include "System.h"
#include "EmptyBehavior.h"
#include "AbstractWindow.h"
#include "MenuWindow.h"
#include "MainMenu.h"
#include "PIDMenu.h"
#include "CarModules.h"

/*
 * Проблема с отображением меню: пустой массив строк, м.б. они не сохраняются
 * Решение:
 * + 1) Создать статический массив хранения для вектора
 * + 2) Добавить setStorage в конструкторе
 */

// #define POLIDOROS_VECTOR

namespace pervansh {
    System* system;
}

String inputString = "";
bool stringComplete = false;
uint8_t ledPin = 23;

float cdCoefs[16] = { 2, 1.40, 1.10, 1.0, 1.0, 1.0, 1.1, 1.1, 1.2, 1.2, 1.2, 1.2, 1.2, 1.2, 1.5, 0.0 };
// float cdCoefs[16] = { 1.6, 1.3, 1.10, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.10, 1.3, 1.6, 0.0 };

MainMenu* mm;
CarBehavior* cdb = new EmptyBehavior();
// CarBehavior* rb = new EmptyBehavior();

class ToCarefulDrivingItem : public MenuWindow::Item {
public:
    ToCarefulDrivingItem(MenuWindow* menu) : MenuWindow::Item(menu) {
        getMenu()->println("To careful mode");
        auto cdMoveModule = new MoveModule(cdb, cdCoefs);
        cdMoveModule->setOctolinerOffset(6); // 7
        cdb->addModule(cdMoveModule);
        cdb->addModule(new SharpModule(cdb));
        cdb->addModule(new IrdaModule(cdb, 9, 10, true));
        // cdb->setEnginePower(25.75);
        auto pid = cdb->getPID();
        /*
        pid->Kp = 45;
        pid->Ki = 3.5;
        pid->Kd = 8;
        */
        /*
        pid->Kp = 31;
        pid->Ki = 11;
        pid->Kd = 6;
        */
        /*
        pid->Kp = 26.5;
        pid->Ki = 13;
        pid->Kd = 18.5;
        */
        /*
        pid->Kp = 35;
        pid->Ki = 15.5;
        pid->Kd = 22.5;
        */
    }

    void onClick() override {
        // getMenu()->getSystem()->setCarBehavior(cdb);
        // mm->getEnginePowerItem()->setEnginePower(cdb->getEnginePower());
    }
};

/*
class ToRacingItem : public MenuWindow::Item {
public:
    ToRacingItem(MenuWindow* menu) : MenuWindow::Item(menu) {
        getMenu()->println("To racing mode");
        float rCoefs[16] = { 1.6, 1.3, 1.10, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.10, 1.3, 1.6, 0.0 };
        auto rMoveModule = new MoveModule(rb, rCoefs);
        rMoveModule->setOctolinerOffset(7);
        rb->addModule(rMoveModule);
        rb->addModule(new IrdaModule(rb, 9, 10));
        rb->setEnginePower(35);
        auto pid = rb->getPID();
    }

    void onClick() override {
        getMenu()->getSystem()->setCarBehavior(rb);
        mm->getEnginePowerItem()->setEnginePower(rb->getEnginePower());
    }
}; */

void setup() {
    Serial.begin(9600);
    //pervansh::system = new System(new EmptyBehavior(10, 11), new MainMenu(pervansh::system));
    pervansh::system = new System();
    pervansh::system->setCarBehavior(cdb);
    mm = new MainMenu(pervansh::system);
    ToCarefulDrivingItem* cdItem = new ToCarefulDrivingItem(mm);
    // ToRacingItem* rItem = new ToRacingItem(mm);
    mm->addItem(cdItem);
    // mm->addItem(rItem);
    pervansh::system->openWindow(mm);
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
}

void loop() {
    pervansh::system->execute();
    if (stringComplete) {
        Serial.println("msg: " + inputString);
        pervansh::system->checkSerialMessage(inputString);
        inputString = "";
        stringComplete = false;
    }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }
}
