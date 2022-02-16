#include <Arduino.h>
#include "MenuWindow.h"
#include "System.h"
#include "config.h"

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
        *value += delta;
        updateData();
        getMenu()->drawItem(ind);
    } else if (getMenu()->isTurnedRight()) {
        *value -= delta;
        updateData();
        getMenu()->drawItem(ind);
    }
}

void MenuWindow::FloatValueItem::updateData() {
    getMenu()->update(valueName + ": " + (String)(*value), ind);
}

void MenuWindow::call() {
    if (holdingItem) {
        holdingItem->onHold();
    } else {
        if (isTurnedLeft() && curItem < items.size() - 1) {
            curItem++;
            curr = max(curr, curItem - config::lcdRowCount + 1);
            draw();
        } else if (isTurnedRight() && curItem > 0) {
            curItem--;
            curr = min(curr, curItem);
            draw();
        }
    }
}


void MenuWindow::print(String s) {
    AbstractWindow::print(" " + s);
}

void MenuWindow::println(String s) {
    AbstractWindow::println(" " + s);
}

void MenuWindow::update(String s, int ind) {
    AbstractWindow::update(" " + s, ind);
}

void MenuWindow::draw() {
    AbstractWindow::draw();
    getLCD()->setCursor(0, curItem - curr);
    getLCD()->print(">");
}

void MenuWindow::draw(int ind) {
    AbstractWindow::draw(ind);
    if (ind == curItem) {
        getLCD()->setCursor(0, curItem - curr);
        getLCD()->print(">");
    }
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
