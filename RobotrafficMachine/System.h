#pragma once

#include "AbstractWindow.h"
#include "CarBehavior.h"
#include "MainMenu.h"

class System {
private:
    CarBehavior* behavior;
    AbstractWindow* window;
public:

    System() {
        window = new MainMenu(this);
    }

    void execute() {
        behavior->execute();
        window->execute();
    }

    void setCarBehavior(CarBehavior* behavior) {this->behavior = behavior;}
    CarBehavior* getCarBehavior() {return behavior;}
    AbstractWindow* getLastWindow() {return window;}
    void openWindow(AbstractWindow* window) {this->window = window;}

    void closeLastWindow() {
        AbstractWindow* prev = window.getLastWindow();

        if (prev) {
            delete window;
            window = prev;
        }
    }

};