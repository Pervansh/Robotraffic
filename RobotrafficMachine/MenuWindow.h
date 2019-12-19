#pragma once

#include "AbstractWindow.h"
#include <Arduino.h>
#include <Vector.h>

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
    };

    class ReturnItem : public Item {
        public:
            ReturnItem(MenuWindow* menu) : Item(menu) {
                menu->println(" Return");
            }

            void onClick() {
                menu->getSystem()->closeLastWindow();
            }
    };

    class FloatValueItem : public Item {
        private:
            bool state = false;
            float* value;
            String valueName;
        public:
            ReturnItem(MenuWindow* menu, float var, String valueName) : Item(menu) {
                this->value = &var;
                this->valueName = valueName;
                menu->println(' ' + valueName + ": " + (String)value);
            }

            void onClick() {
                if (state) {
                    menu->setHoldingItem(nullptr);
                } else {
                    menu->setHoldingItem(this);
                }
                state = !state;
            }
            
            void onHold() override {
                if (menu->isTurnedRight()) {
                    *value += 0.05f; 
                } else if (menu->isTurnedLeft()) {
                    *value -= 0.05f;
                }
            }
    };

protected:
    Vector<Item> items;
    Item* holdingItem;
    int curItem = 0;

public:
    virtual void call();
    
    virtual void onClick() override {
        items[curItem].onClick();
    }

    void addItem(Item item) {
        item.ind = items.size();
        items.push_back(item);
    }

    void setHoldingItem(Item* item) {
        holdingItem = item;
        useScrolling(!item);
    }

    ~MenuWindow() {
        for (Item item : items) {
            delete item;
        }
    }
};