#pragma once

#include <Arduino.h>
#include <Vector.h>
#include "AbstractWindow.h"

class MenuWindow : public AbstractWindow {
public:
    class Item {
    protected:
        MenuWindow* menu;
    public:
        int ind;

        Item(MenuWindow* menu) {
            this->menu = menu;
        }

        virtual void onClick() = 0;
        virtual void onHold() {}
        MenuWindow* getMenu() {return menu;}
    };

    class ReturnItem : public Item {
        public:
            ReturnItem(MenuWindow* menu) : Item(menu) {
                getMenu()->println(" Return");
            }

            void onClick();
    };

    class FloatValueItem : public Item {
        private:
            bool state = false;
            float* value;
            String valueName;
        public:
            FloatValueItem(MenuWindow* menu, float* var, String valueName) : Item(menu) {
                Serial.println("FVI: s");
                this->value = var;
                Serial.println("FVI: var");
                this->valueName = valueName;
                Serial.println("FVI: vn");
                getMenu()->println(' ' + valueName + ": " + (String)*value);
                Serial.println("FVI: e");
            }

            void onClick();
            
            void onHold() override;
    };

protected:
    Vector<Item*> items;
    Item* holdingItem;
    int curItem = 0;

public:
    MenuWindow(System* system, AbstractWindow* prev = nullptr) : AbstractWindow(system, prev) {
        holdingItem = nullptr;
        useScrolling(false);
    }
    
    virtual void call();
    virtual void draw();
    virtual void readCommand(String);
    
    virtual void onClick() override {
        if (items.size() > 0) {
            items[curItem]->onClick();
        }
    }

    void addItem(Item* item) {
        item->ind = items.size();
        items.push_back(item);
        draw();
    }

    void setHoldingItem(Item* item) {
        holdingItem = item;
        //useScrolling(!item);
    }

    ~MenuWindow() {
        for (int i = 0; i < items.size(); i++) {
            delete items[i];
            Serial.println("MN - delete[" + (String)i + "]");
        }
    }
};
