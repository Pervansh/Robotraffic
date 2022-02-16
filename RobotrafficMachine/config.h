#pragma once
#include <inttypes.h>

namespace config {
    const uint8_t lcdRowCount = 4;
    const uint8_t lcdColumnCount = 20;
    const uint8_t angleServoPin = 5;
    const uint8_t speedServoPin = 6;
    const uint8_t sharpPin = A22;
    const uint16_t sharpReductionRange = 190;
    const uint16_t sharpBreakRange = 60;
    const uint16_t cityProporcialEeprom = 0;
    const uint16_t cityIntegralEeprom = 4;
    const uint16_t cityDifferenceEeprom = 8;
    const float fullSpeed = 75;
    const float enginePowerChangeDelta = 0.5;
}
