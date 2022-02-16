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
    void setCarBehavior(CarBehavior* behavior);
    CarBehavior* getCarBehavior() {return behavior;}
    AbstractWindow* getLastWindow() {return window;}
    void openWindow(AbstractWindow*);

    void closeLastWindow();
};
