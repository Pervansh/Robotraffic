#include <Arduino.h>
#include "MenuWindow.h"
#include "System.h"

void MenuWindow::ReturnItem::onClick() {
    getMenu()->close();
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
    if (getMenu()->isTurnedLeft()) {
        *value += 0.05f;
        getMenu()->update(' ' + valueName + ": " + (String)*value, ind);
        getMenu()->draw();
    } else if (getMenu()->isTurnedRight()) {
        *value -= 0.05f;
        getMenu()->update(' ' + valueName + ": " + (String)*value, ind);
        getMenu()->draw();
    }
}

void MenuWindow::call() {
    if (holdingItem) {
        holdingItem->onHold();
    } else {
        if (isTurnedLeft() && curItem < items.size() - 1) {
            curItem++;
            curr = max(curr, curItem - 3);
            draw();
        } else if (isTurnedRight() && curItem > 0) {
            curItem--;
            curr = min(curr, curItem);
            draw();
        }
    }
}

void MenuWindow::draw() {
    AbstractWindow::draw();
    getLCD()->setCursor(0, curItem - curr);
    getLCD()->print(">");
}

void MenuWindow::readCommand(String msg) {
    if (msg.equals(">")) {
        onClick();
    } else if (holdingItem == nullptr) {
        AbstractWindow::readCommand(msg);
        int ind = msg.toInt() - 1;
        curItem = constrain(ind, 0, items.size() - 1);
    }
}
