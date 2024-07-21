#pragma once
#ifndef HW_ISR_HPP
#define HW_ISR_HPP
#include "ESP32TimerInterrupt.hpp"

namespace isr{
    extern ESP32Timer BtnTimer;  // declaration of the global variable ISRTimer
    extern ESP32Timer LedBlinkTimer;  // declaration of the global variable ISRTimer
}
#endif