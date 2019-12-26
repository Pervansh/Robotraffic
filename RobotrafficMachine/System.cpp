#include <Arduino.h>
#include "System.h"
#include "AbstractWindow.h"
#include "MainMenu.h"
#include "CarBehavior.h"

System::System() {
    window = nullptr;
    behavior = nullptr;
}

void System::execute() {
    //behavior->execute();
    window->execute();
    //Serial.println("system - execute");
}

void System::checkSerialMessage(String msg) {
    if (msg.equals("!ping")) {
        Serial.println("System is working.");
    } else if (!msg.equals("")) {
        window->readCommand(msg);
    }
}

void System::openWindow(AbstractWindow* window) {
    this->window = window;
    window->draw();
    Serial.println("Window was opened");
}

void System::closeLastWindow() {
    AbstractWindow* prev = window->getPreviousWindow();
    
    if (prev) {
        Serial.println("system - closeWindow: prev = " + (String)((int)prev));
        delete window;
        Serial.println("system - closeWindow: window deleted");
        window = prev;
        window->draw();
    }
}
