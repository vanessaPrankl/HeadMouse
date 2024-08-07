#pragma once

#include "def_general.hpp"

constexpr uint32_t LED_BLINK_INTERVAL_MS = 500;  // LED blink duration 
constexpr uint32_t LED_COUNT = 2;
constexpr uint32_t LED_PIN_COUNT = 2;

/*! *********************************************************
* @brief Enum to define available LED states
*************************************************************/
enum ledState {
    RED,
    GREEN,
    ORANGE,
    BLINK_RED,
    BLINK_GREEN,
    BLINK_ORANGE
};

/*! *********************************************************
* @brief Enum to define available LEDs
*************************************************************/
enum ledType {
    LED_BATTERY,
    LED_STATUS
};

namespace led{

    struct ledConfig{
        ledState state; 
        pin pin_g;
        pin pin_r;

        // Constructor for ledConfig
        ledConfig(ledState state, pin g, pin r)
            : state(state), pin_g(g), pin_r(r) {}
    };


    extern volatile ledConfig _config[2];

    static void initConfig(pin battery_pin_g, pin battery_pin_r, pin status_pin_g, pin status_pin_r){
        _config[LED_BATTERY].pin_g = battery_pin_g;
        _config[LED_BATTERY].pin_r = battery_pin_r;
        _config[LED_STATUS].pin_g = status_pin_g;
        _config[LED_STATUS].pin_r = status_pin_r;
    }
}

using namespace led;


/*! *********************************************************
* @brief Class to handle LED actions
*************************************************************/
class Leds {
private: 
    static Leds* instance; // Static instance pointer for singleton   
    
    Leds(pin battery_pin_g, pin battery_pin_r, pin status_pin_g, pin status_pin_r){
        initConfig(battery_pin_g, battery_pin_r, status_pin_g, status_pin_r);
    }

    static bool _callbackTimerLed(void *);
   
public:
    // Static method to get the singleton instance
    static Leds* getInstance(pin battery_pin_g, pin battery_pin_r, pin status_pin_g, pin status_pin_r);
    err init();
    void set(ledType, ledState);
};