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
    static LiquidCrystal_I2C* lcd;
    static Encoder* encoder;
    long drawTimer;
    long drawDelta;
    bool isDrawOnTimer;
    bool isClosing = false;
    bool isRight = false, isLeft = false, isFastL = false, isFastR = false;
    bool isLeftIgnore = false, isRightIgnore = false;
    int leftCounter = 0;
    int rightCounter = 0;

protected:
    System* system;
    Vector<String> strings;
    int curr;
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
    virtual void close();

    virtual void readCommand(String);
    void useScrolling(bool b) {isScrolling = b;}
    void useDrawOnTimer(bool use) {isDrawOnTimer = use;}
    void setDrawDelta(long dd) {drawDelta = dd;}
    bool isTurnedRight() {return rightCounter >= 2;}
    bool isTurnedLeft() {return leftCounter >= 2;}
    bool isFastTurnedRight() {return isFastR;}
    bool isFastTurnedLeft() {return isFastL;}
    System* getSystem() {return system;}
    static LiquidCrystal_I2C* getLCD() {return lcd;}
    static Encoder* getEncoder() {return encoder;}
    AbstractWindow* getPreviousWindow() {return prev;}

};
