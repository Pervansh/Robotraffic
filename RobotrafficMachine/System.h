#pragma once

#include <Arduino.h>

class String;
class AbstractWindow;
class CarBehavior;

class System {
private:
    CarBehavior* behavior;
    AbstractWindow* window;
public:

    System();

    void execute();

    void checkSerialMessage(String);
    void setCarBehavior(CarBehavior* behavior) {
        if (this->behavior) {
            delete this->behavior;
        }
        this->behavior = behavior;
        Serial.println("Behavior was changed");
    }
    CarBehavior* getCarBehavior() {return behavior;}
    AbstractWindow* getLastWindow() {return window;}
    void openWindow(AbstractWindow*);

    void closeLastWindow();
};
