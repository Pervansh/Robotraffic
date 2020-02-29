#include <SoftwareSerial.h>

#define RX 11
#define TX 10
#define RED 4
#define GREEN 5
#define BLUE 6

SoftwareSerial stream(RX, TX);

void setup() {
    stream.begin(115200);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop() {
    if (millis() % 15000 <= 7500) {
        stream.write(1);
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, HIGH);
        digitalWrite(GREEN, HIGH);
    } else if (millis() % 15000 <= 12500) {
        stream.write(2);
        digitalWrite(RED, HIGH);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, HIGH);
    } else {
        stream.write(3);
        digitalWrite(RED, LOW);
        digitalWrite(BLUE, LOW);
        digitalWrite(GREEN, HIGH);
    }
}
