#pragma once
#include "ESP32TimerInterrupt.hpp"

namespace isr{
    extern ESP32Timer BtnTimer;             /* Timer 0 */ 
    extern ESP32Timer LedBlinkTimer;        /* Timer 1 */ 
    extern ESP32Timer ProgramCycleTimer;    /* Timer 2 */ 
}
