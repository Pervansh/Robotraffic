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
            getMenu()->draw();
        } else {
            getMenu()->setHoldingItem(this);
            getMenu()->getSystem()->getCarBehavior()->run();
            getMenu()->update(" Launch: ON", ind);
            getMenu()->draw();
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

class TextWindow : public AbstractWindow {
public:
    TextWindow(System* system, AbstractWindow* prev = nullptr) : AbstractWindow(system, prev){
        println("1");
        println("2");
        println("3");
        println("4");
        println("5");
        println("6");
        println("7");
        println("8");
    }

    void call() {}
};

class OpenTextItem : public MenuWindow::Item {
public:
    OpenTextItem(MenuWindow* menu) : MenuWindow::Item(menu) {
        getMenu()->println(" Text");
    }
           
    void onClick() {
        TextWindow* tw = new TextWindow(getMenu()->getSystem(), getMenu());
        getMenu()->getSystem()->openWindow(tw);
    } 
};

MainMenu::MainMenu(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
    addItem(new LaunchCarItem(this));
    addItem(new OpenPIDMenuItem(this));
    addItem(new OpenTextItem(this));
}
