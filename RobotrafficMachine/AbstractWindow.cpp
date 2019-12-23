#pragma once
#include "AbstractWindow.h"
#include "System.h"
#include <Arduino.h>
#include <Vector.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

class String;

LiquidCrystal_I2C AbstractWindow::lcd = LiquidCrystal_I2C(0x27, 20, 4);
Encoder AbstractWindow::encoder = Encoder(2, 3, 4, true);

AbstractWindow::AbstractWindow(System* system, AbstractWindow* prev = nullptr) {
    this->system = system;
    this->prev = prev;
    curr = 0;
    isScrolling = true;
}

AbstractWindow::~AbstractWindow(){
    flush();
}

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
    int j = 0, i = min(curr, strings.size() - 4);
    for (; i < min(curr + 4, strings.size()); i++, j++) {
        lcd.setCursor(0, j);
        lcd.print(strings[curr]);
    }
}

void AbstractWindow::onClick() {
    system->closeLastWindow();
}

void AbstractWindow::execute() {
    isLeft = encoder.isLeft();
    isRight = encoder.isRight();

    if (encoder.isClick()) {
        onClick();
    }

    call();

    if (isScrolling) {
        if (isRight && curr <= strings.size() - 4) {
            curr++;
        } else if (isLeft && curr >= 0) {
            curr--;
        }
    }
    
    draw();
}
