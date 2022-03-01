#pragma once

#include "MenuWindow.h"
#include "config.h"
#include "CarBehavior.h"
#include "System.h"

class EnginePowerItem;

class MainMenu : public MenuWindow {
private:
    EnginePowerItem* enginePowerItem;
public:
    MainMenu(System* system, AbstractWindow* prev = nullptr);

    EnginePowerItem* getEnginePowerItem() { return enginePowerItem; }
};

class EnginePowerItem : public MenuWindow::FloatValueItem {
private:
    float enginePower;
public:
    EnginePowerItem(MainMenu* menu)
    : MenuWindow::FloatValueItem(menu, &enginePower, config::enginePowerChangeDelta, "En. pow.") {
        enginePower = getMenu()->getSystem()->getCarBehavior()->getEnginePower();
        updateData(); // IDK WHY IT'S GENERETING AN ERROR (now it doesn't)!!!
    }

    void onClick() {
        MenuWindow::FloatValueItem::onClick();
        if (getMenu()->getHoldingItem() == nullptr) {
            setEnginePower(enginePower);
        }
    }

    float getEnginePower() { return enginePower; }
    void setEnginePower(float newEnginePower) {
        getMenu()->getSystem()->getCarBehavior()->setEnginePower(enginePower);
        enginePower = newEnginePower;
    }
};
