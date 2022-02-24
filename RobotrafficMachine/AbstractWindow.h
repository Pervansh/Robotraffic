#pragma once
#include <Arduino.h>
#include <Vector.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <GyverEncoder.h>

#define MIN_TURN_LEFT_TRIGGER 1  // 2
#define MIN_TURN_RIGHT_TRIGGER 1  // 2

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

    #ifdef POLIDOROS_VECTOR
        const int MAX_STRINGS = 32;
        String stringStorage[MAX_STRINGS];
    #endif

public:
    AbstractWindow(System* system, AbstractWindow* prev = nullptr);

    void execute(); // reading info, uses call-function
    void flush();
    virtual void print(String);
    virtual void println(String);
    virtual void update(String, int);
    virtual void draw();
    virtual void draw(int);
    virtual void close();

    virtual void readCommand(String);
    void useScrolling(bool b) {isScrolling = b;}
    void useDrawOnTimer(bool use) {isDrawOnTimer = use;}
    void setDrawDelta(long dd) {drawDelta = dd;}
    bool isTurnedRight() {return rightCounter >= MIN_TURN_RIGHT_TRIGGER;}
    bool isTurnedLeft() {return leftCounter >= MIN_TURN_LEFT_TRIGGER;}
    bool isFastTurnedRight() {return isFastR;}
    bool isFastTurnedLeft() {return isFastL;}
    System* getSystem() {return system;}
    static LiquidCrystal_I2C* getLCD() {return lcd;}
    static Encoder* getEncoder() {return encoder;}
    AbstractWindow* getPreviousWindow() {return prev;}

    virtual ~AbstractWindow();
};
