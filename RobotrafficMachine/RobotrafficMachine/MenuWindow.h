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

        virtual ~Item() = default;
    };

    class ReturnItem : public Item {
        public:
            ReturnItem(MenuWindow* menu) : Item(menu) {
                getMenu()->println("Return");
            }

            virtual void onClick();
    };

    class FloatValueItem : public Item {
        private:
            bool state = false;
            float* value;
            float delta;
            String valueName;
        public:
            FloatValueItem(MenuWindow* menu, float* var, float changeDelta, String valueName) : Item(menu) {
                this->value = var;
                this->delta = changeDelta;
                this->valueName = valueName;
                getMenu()->println(valueName + ": " + (String)*value);
            }

            virtual void onClick();
            
            virtual void onHold() override;

            virtual void updateData();
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
    virtual void print(String);
    virtual void println(String);
    virtual void update(String, int);
    virtual void draw();
    virtual void draw(int);
    virtual void readCommand(String);
    
    virtual void onClick() override {
        if (items.size() > 0) {
            items[curItem]->onClick();
        }
    }

    void drawItem(int ind) {
        draw(ind - curr);
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
    
    Item* getHoldingItem() {return holdingItem;}

    ~MenuWindow() {
        for (int i = 0; i < items.size(); i++) {
            delete items[i];
        }
    }
};
