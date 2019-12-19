#pragma once
#include "AbstractWindow.h"
#include <Arduino.h>
#include <Vector.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

class String;

int min(int, int);

void AbstractWindow::print(String s) {
    int ind = strings.size() - 1;
    strings[ind] += s;
}

void AbstractWindow::println(String s) {
    strings.push_back(s);
}

void AbstractWindow::flush() {
    strings = Vector<String>();
    lcd.clear();
}

void AbstractWindow::update(String s, int ind) {
    strings[ind] = s;
}

void AbstractWindow::draw() {
    lcd.clear();
    for (int j = 0, int i = min(curr, strings.size() - 4); i < min(curr + 4, strings.size()); i++, j++) {
        lcd.setCursor(0, j);
        lcd.print(strings[curr]);
    }
}

void AbstractWindow::onClick() {
    closeLastWindow();
}

void AbstractWindow::execute() {
    isLeft = encoder.isLeft();
    isRight = encoder.isRight();

    if (encoder.isClick()) {
        onClick();
    }

    call();

    if (isScroling) {
        if (isRight && cur <= strings.size() - 4) {
            cur++;
        } else if (isLeft && cur >= 0) {
            cur--;
        }
    }
    
    draw();
}