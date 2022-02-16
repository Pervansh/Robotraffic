#pragma once

#include <Arduino.h>
#include <GyverPID.h>
#include "MenuWindow.h"
#include "System.h"
#include "CarBehavior.h"

class PIDMenu : public MenuWindow {
private:

    class SafePidItem : public MenuWindow::Item {
    public:
        SafePidItem(MenuWindow* menu) : MenuWindow::Item(menu) {
            getMenu()->println("Safe PID in EEPROM");
        }

        void onClick() {
            class ConfirmWindow : public AbstractWindow {
            public:
                ConfirmWindow(System* system, MenuWindow* menu) : AbstractWindow(system, menu) {
                    println("PID was saved!");
                }
                void call(){}
            };
            getMenu()->getSystem()->getCarBehavior()->safePidInEeprom();
            getMenu()->getSystem()->openWindow(new ConfirmWindow(getMenu()->getSystem(), getMenu()));
        }
    };
public:
    PIDMenu(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
        GyverPID* pid = getSystem()->getCarBehavior()->getPID();
        addItem(new MenuWindow::FloatValueItem(this, &pid->Kp, 0.5f, "P"));
        addItem(new MenuWindow::FloatValueItem(this, &pid->Ki, 0.5f, "I"));
        addItem(new MenuWindow::FloatValueItem(this, &pid->Kd, 0.5f, "D"));
        addItem(new SafePidItem(this));
        addItem(new MenuWindow::ReturnItem(this));
    }
};
