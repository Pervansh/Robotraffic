#pragma once
#include <Arduino.h>
#include <Vector.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

class String;
class System;

class AbstractWindow {
private:
    AbstractWindow* prev;
    static LiquidCrystal_I2C lcd;
    static Encoder encoder;

protected:
    System* system;
    Vector<String> strings;
    int curr;
    bool isRight = false, isLeft = false;
    bool isScrolling;
    virtual void onClick();
    virtual void call() = 0;

public:
    AbstractWindow(System* system, AbstractWindow* prev = nullptr);

    ~AbstractWindow();

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
