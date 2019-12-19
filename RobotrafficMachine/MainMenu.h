#pragma once

#include "MenuWindow.h"
#include "PIDMenu.h"

class MainMenu : public MenuWindow {
private:
    class LaunchCarItem : public MenuWindow::Item {
        private:
            bool state = false;
        public:
            LaunchCarItem(MenuWindow* menu) : MenuWindow::Item(menu) {
                menu->println(" Launch: OFF");
            }

            void onClick(){
                if (state) {
                    menu->setHoldingItem(nullptr);
                    menu->getSystem()->getCarBehavior()->stop();
                    menu->update(" Launch: OFF", ind);
                } else {
                    menu->setHoldingItem(this);
                    menu->getSystem()->getCarBehavior()->run();
                    menu->update(" Launch: ON", ind);
                }
                state = !state;
            }
    };

    class OpenPIDMenuItem : public MenuWindow::Item {
        public:
            OpenPIDMenuItem(MenuWindow* menu) : MenuWindow::Item(menu) {
                menu->println(" PID Settings");
            }

            void onClick() {
                PIDMenu* pm = new PIDMenu(menu->getSystem, menu);
                menu->getSystem()->openWindow(pm);
            } 
    }


public:
    MainMenu(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
        addItem(LaunchCarItem(this));
        addItem(OpenPIDMenuItem(this));
    }
};
