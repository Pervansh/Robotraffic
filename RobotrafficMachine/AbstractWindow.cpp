#include "AbstractWindow.h"
#include "System.h"
#include <Arduino.h>
#include <Vector.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

class String;

#if defined(__AVR_ATmega328__)
    uint8_t attachPCINT(uint8_t);
#endif

LiquidCrystal_I2C* AbstractWindow::lcd = nullptr;
Encoder* AbstractWindow::encoder = nullptr;



AbstractWindow::AbstractWindow(System* system, AbstractWindow* prev = nullptr) {
    this->system = system;
    this->prev = prev;
    if (!lcd) {
        lcd = new LiquidCrystal_I2C(0x27, 20, 4);
        lcd->init();
        lcd->backlight();
    }
    if (!encoder) {
        encoder = new Encoder(29, 34, 35, false);  //false - double turn, true - bad left working
    }
    if (encoder->isClick()) {
        encoder->tick();
    }
    curr = 0;
    isScrolling = true;
    isDrawOnTimer = false;
    drawTimer = 0;
    drawDelta = 300;
    
    // настроить PCINT
    #if defined(__AVR_ATmega328__)
        attachPCINT(2); //cl
        attachPCINT(3); //dt
    #endif
    draw();
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
    lcd->clear();
}

void AbstractWindow::update(String s, int ind) {
    strings[ind] = s;
}

void AbstractWindow::draw() {
    lcd->clear();
    int j = 0, i = max(min(curr, strings.size() - 4), 0);
    for (; i < min(curr + 4, strings.size()); i++, j++) {
        lcd->setCursor(0, j);
        lcd->print(strings[i]);
    }
}

void AbstractWindow::draw(int ind) {
    if (ind >= 0 && ind < 4 && ind + curr < strings.size()) {
        String str = strings[curr + ind];
        for (int i = strings[ind].length(); i < 20; i++) {
            str += ' ';
        }
        lcd->setCursor(0, ind);
        lcd->print(str);
    }
}

void AbstractWindow::onClick() {
    close();
}

void AbstractWindow::readCommand(String msg) {
    int ind = msg.toInt() - 1;
    if (isScrolling) {
        curr = constrain(ind, 0, strings.size() - 4);
    }
}

void AbstractWindow::close() {
    isClosing = true;
}

void AbstractWindow::execute() {
    encoder->tick();
    isLeft = encoder->isLeft();
    isRight = encoder->isRight();
    isFastL = encoder->isFastL() && !isLeftIgnore;
    isFastR = encoder->isFastR() && !isRightIgnore;
    isLeftIgnore = isFastL;
    isRightIgnore = isFastR;
    if (isLeft) {
        leftCounter++;
    }
    if (isRight) {
        rightCounter++;
    }
    
    if (encoder->isClick()) {
        onClick();
    }

    call();

    if (isScrolling) {
        if (isTurnedLeft() && curr < strings.size() - 4) {
            curr++;
            draw();
        } else if (isTurnedRight() && curr > 0) {
            curr--;
            draw();
        }
    }

    if (isDrawOnTimer && millis() - drawTimer >= drawDelta) {
        drawTimer = millis(); 
        draw();
    }

    leftCounter = leftCounter % 2;
    rightCounter = rightCounter % 2;

    if (isClosing) {
        system->closeLastWindow();
    }
}

#if defined(__AVR_ATmega328__)
// функция для настройки PCINT для ATmega328 (UNO, Nano, Pro Mini)
uint8_t attachPCINT(uint8_t pin) {
  if (pin < 8) { // D0-D7 // PCINT2
    PCICR |= (1 << PCIE2);
    PCMSK2 |= (1 << pin);
    return 2;
  }
  else if (pin > 13) { //A0-A5  // PCINT1
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << pin - 14);
    return 1;
  }
  else { // D8-D13  // PCINT0
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << pin - 8);
    return 0;
  }
}

// Векторы PCINT, нужно кинуть сюда тики
// не обязательно в каждый вектор, достаточно в тот, который задействован
// пины 0-7: PCINT2
// пины 8-13: PCINT0
// пины A0-A5: PCINT1
ISR(PCINT0_vect) {
  //enc1.tick();
}
ISR(PCINT1_vect) {
  //enc1.tick();
}
ISR(PCINT2_vect) {
  AbstractWindow::getEncoder()->tick();
}

#endif
