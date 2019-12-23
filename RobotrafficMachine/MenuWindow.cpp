#include <Arduino.h>
#include "MenuWindow.h"
#include "System.h"

void MenuWindow::ReturnItem::onClick() {
    getMenu()->getSystem()->closeLastWindow();
}

void MenuWindow::FloatValueItem::onClick() {
    if (state) {
        getMenu()->setHoldingItem(nullptr);
    } else {
        getMenu()->setHoldingItem(this);
    }
    state = !state;
}

void MenuWindow::FloatValueItem::onHold(){
    if (getMenu()->isTurnedRight()) {
        *value += 0.05f; 
    } else if (getMenu()->isTurnedLeft()) {
        *value -= 0.05f;
    }
    getMenu()->update('\x84' + valueName + ": " + (String)*value, ind);
}

void MenuWindow::call() {
    if (holdingItem) {
        holdingItem->onHold();
    } else {
        update('\x84' + strings[curItem].substring(1, strings[curItem].length() - 1), items[curItem]->ind);
        if (isRight && curItem < items.size()) {
            update(' ' + strings[curItem].substring(1, strings[curItem].length() - 1), items[curItem]->ind);
            curItem++;
        } else if (isLeft && curItem >= 0) {
            update(' ' + strings[curItem].substring(1, strings[curItem].length() - 1), items[curItem]->ind);
            curItem--;
        }
    }
}
