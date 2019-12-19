#include "MenuWindow.h"

void MenuWindow::call() {
    if (holdingItem) {
        holdingItem->onHold();
    } else {
        update('\x84' + strings[curItem].substring(1, strings[curItem].size() - 1));
        if (isRight && curItem < items.size()) {
            update(' ' + strings[curItem].substring(1, strings[curItem].size() - 1));
            curItem++;
        } else if (isLeft && curItem >= 0) {
            update(' ' + strings[curItem].substring(1, strings[curItem].size() - 1));
            curItem--;
        }
    }
}