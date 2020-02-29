#include "System.h"
#include "EmptyBehavior.h"
#include "AbstractWindow.h"
#include "MenuWindow.h"
#include "MainMenu.h"
#include "PIDMenu.h"
#include "CarModules.h"



float a = 20;

class AS : public MenuWindow {
    public:
        class IT : public MenuWindow::Item {
            public:
            IT(MenuWindow* menu, String str) : MenuWindow::Item(menu) {
                getMenu()->println(str);
            }

            void onClick(){}
        };
    
        AS(System* system, AbstractWindow* prev = nullptr) : MenuWindow(system, prev) {
            addItem(new MenuWindow::FloatValueItem(this, &a, "a"));
        }
};

class AW : public AbstractWindow {
    public:
        AW(System* system, AbstractWindow* prev = nullptr) : AbstractWindow(system, prev) {
            println("1");
            println("2");
            println("3");
            println("4");
            println("5");
            println("6");
            println("7");
            println("8");
            println("9");
            println("10");
        }
        
        void call(){}
};

namespace pervansh {
    System* system;
}

String inputString = "";
bool stringComplete = false;

void setup() {
    Serial.begin(9600);
    //pervansh::system = new System(new EmptyBehavior(10, 11), new MainMenu(pervansh::system));
    pervansh::system = new System();
    CarBehavior* cb = new EmptyBehavior();
    cb->addModule(new SharpModule(cb));
    pervansh::system->setCarBehavior(cb);
    delay(500);
    MainMenu* mm = new MainMenu(pervansh::system);
    pervansh::system->openWindow(mm);
}

void loop() {
    pervansh::system->execute();
    if (stringComplete) {
        Serial.println("msg: " + inputString);
        pervansh::system->checkSerialMessage(inputString);
        inputString = "";
        stringComplete = false;
    }
}

void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        if (inChar == '\n') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }
}
