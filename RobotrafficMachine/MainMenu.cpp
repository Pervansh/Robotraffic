#include "MenuWindow.h"
#include "MainMenu.h"
#include "PIDMenu.h"
#include "System.h"
#include "CarBehavior.h"

class LaunchCarItem : public MenuWindow::Item {
private:
    bool state = false;
public:
    LaunchCarItem(MenuWindow* menu) : MenuWindow::Item(menu) {
        getMenu()->println(" Launch: OFF");
    }
     
    void onClick(){
        if (state) {
            getMenu()->setHoldingItem(nullptr);
            getMenu()->getSystem()->getCarBehavior()->stop();
            getMenu()->update(" Launch: OFF", ind);
        } else {
            getMenu()->setHoldingItem(this);
            getMenu()->getSystem()->getCarBehavior()->run();
            getMenu()->update(" Launch: ON", ind);
        }
        state = !state;
    }
};

class OpenPIDMenuItem : public MenuWindow::Item {
public:
    OpenPIDMenuItem(MenuWindow* menu) : MenuWindow::Item(menu) {
        getMenu()->println(" PID Settings");
    }
           
    void onClick() {
        PIDMenu* pm = new PIDMenu(getMenu()->getSystem(), getMenu());
        getMenu()->getSystem()->openWindow(pm);
    } 
};

MainMenu::MainMenu(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
    addItem(new LaunchCarItem(this));
    addItem(new OpenPIDMenuItem(this));
}
