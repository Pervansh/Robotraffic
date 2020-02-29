#pragma once
#include <inttypes.h>

namespace config {
    const uint8_t angleServoPin = 5;
    const uint8_t speedServoPin = 6;
    const uint8_t sharpPin = A22;
    const uint16_t cityProporcialEeprom = 0;
    const uint16_t cityIntegralEeprom = 4;
    const uint16_t cityDifferenceEeprom = 8;
    const float fullSpeed = 75;
}
