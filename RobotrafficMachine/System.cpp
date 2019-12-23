#include "System.h"
#include "AbstractWindow.h"
#include "MainMenu.h"
#include "CarBehavior.h"

System::System(CarBehavior* cb, AbstractWindow* aw) {
    window = aw;
    behavior = cb;
}

void System::execute() {
    behavior->execute();
    window->execute();
}

void System::closeLastWindow() {
    AbstractWindow* prev = window->getPreviousWindow();
    
    if (prev) {
        delete window;
        window = prev;
    }
}
