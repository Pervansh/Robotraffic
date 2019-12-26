#pragma once

#include <Arduino.h>
#include <GyverPID.h>
#include "MenuWindow.h"
#include "System.h"
#include "CarBehavior.h"

class PIDMenu : public MenuWindow {
private:
    
public:
    PIDMenu(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
        GyverPID* pid = getSystem()->getCarBehavior()->getPID();
        float a = 0;
        addItem(new MenuWindow::FloatValueItem(this, &pid->Kp, "P"));
        addItem(new MenuWindow::FloatValueItem(this, &pid->Ki, "I"));
        addItem(new MenuWindow::FloatValueItem(this, &pid->Kd, "D"));
        addItem(new MenuWindow::FloatValueItem(this, &a, "just a"));
        addItem(new MenuWindow::ReturnItem(this));
    }
};
