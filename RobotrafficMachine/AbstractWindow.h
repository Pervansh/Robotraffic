#pragma once
#include <Arduino.h>
#include "System.h"
#include <Vector.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

class String;

class AbstractWindow {
private:
    AbstractWindow* prev;
    static LiquidCrystal_I2C lcd(0x27, 20, 4);
    static GyverEncoder encoder(2, 3);

protected:
    System* system;
    Vector<String> strings;
    int cur;
    bool isRight = false, isLeft = false;
    bool isScrolling;
    virtual void onClick();
    virtual void call() = 0;

public:
    AbstractWindow(System* system, AbstractWindow* prev = nullptr) {
        this->system = system;
        this->prev = prev;
        cur = 0;
        isScrolling = true;
        lastWindow = this;
    }

    ~AbstractWindow(){
        flush();
    }

    void execute(); // reading info, uses call-function
    void flush();
    void print(String);
    void println(String);
    void update(String, int);
    virtual void draw();

    void useScrolling(bool b) {
        isScrolling = b;
    }

    bool isTurnedRight() {return isRight;}
    bool isTurnedLeft() {return isLeft;}
    System* getSystem() {return system;}
    static LiquidCrystal_I2C* getLCD() {return &lcd;}
    AbstractWindow* getPreviousWindow() {return prev;}

};
