#pragma once

#include "ESP32TimerInterrupt.h"

namespace isr{
    ESP32Timer BtnTimer(0); /* HW timer for button push detection */
    ESP32Timer LedBlinkTimer(1); /* HW timer for LED blink control */
}
