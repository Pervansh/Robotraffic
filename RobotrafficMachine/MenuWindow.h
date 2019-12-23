#pragma once

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
            FloatValueItem(MenuWindow* menu, float var, String valueName) : Item(menu) {
                this->value = &var;
                this->valueName = valueName;
                getMenu()->println(' ' + valueName + ": " + (String)*value);
            }

            void onClick();
            
            void onHold() override;
    };

protected:
    Vector<Item*> items;
    Item* holdingItem;
    int curItem = 0;

public:
    MenuWindow(System* system, AbstractWindow* prev = nullptr) : AbstractWindow(system, prev) {}
    
    virtual void call();
    
    virtual void onClick() override {
        items[curItem]->onClick();
    }

    void addItem(Item* item) {
        item->ind = items.size();
        items.push_back(item);
    }

    void setHoldingItem(Item* item) {
        holdingItem = item;
        useScrolling(!item);
    }

    ~MenuWindow() {
        for (int i = 0; i < items.size(); i++) {
            delete items[i];
        }
    }
};
