#pragma once

class AbstractWindow;
class CarBehavior;

class System {
private:
    CarBehavior* behavior;
    AbstractWindow* window;
public:

    System(CarBehavior*, AbstractWindow*);

    void execute();

    void setCarBehavior(CarBehavior* behavior) {this->behavior = behavior;}
    CarBehavior* getCarBehavior() {return behavior;}
    AbstractWindow* getLastWindow() {return window;}
    void openWindow(AbstractWindow* window) {this->window = window;}

    void closeLastWindow();
};
