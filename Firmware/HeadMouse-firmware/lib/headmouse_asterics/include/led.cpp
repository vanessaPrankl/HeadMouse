#include <Arduino.h>
#include "led.hpp"
#include "logging.hpp"
#include "hw_isr.hpp"
#include "def_general.hpp"

namespace led{
    volatile ledConfig _config[2] = {{RED, 0, 0}, {RED, 0, 0}};
}
using namespace led;


namespace isr{
    ESP32Timer LedBlinkTimer(1);
}
using namespace isr;



/* Define the static instance pointer */
Leds* Leds::instance = nullptr;

/************************************************************

 * @brief Get the singleton instance of the Leds class.
 *
 * This function initializes and returns the singleton instance of the Leds class. 
 * If the instance does not already exist, it creates a new one.
 *
 * @param pin_status_g The pin number for the green status LED.
 * @param pin_status_r The pin number for the red status LED.
 * @param pin_battery_g The pin number for the green battery LED.
 * @param pin_battery_r The pin number for the red battery LED.
 * @return A pointer to the singleton instance of the Leds class.
 *************************************************************/
Leds* Leds::getInstance(pin pin_status_g, pin pin_status_r, pin pin_battery_g, pin pin_battery_r) {
    if (instance == nullptr) {
        instance = new Leds(pin_status_g, pin_status_r, pin_battery_g, pin_battery_r);
    }
    return instance;
}


/************************************************************
 * @brief Initialize the LEDs.
 *
 * This function initializes the LED pins and sets up the timer interrupt for LED blinking.
 *
 * @return ERR_NONE if initialization is successful, otherwise ERR_GENERIC.
 *************************************************************/
err Leds::init(){
    err error = ERR_GENERIC;
    for(int i=0; i++; i<LED_COUNT){
        pinMode(_config[i].pin_r, OUTPUT);
        pinMode(_config[i].pin_g, OUTPUT);
    }

    if (LedBlinkTimer.attachInterruptInterval(LED_BLINK_INTERVAL_MS * 1000, _callbackTimerLed)){
        error = ERR_NONE;
    }
    else return ERR_GENERIC;
    
    return error;
}

/************************************************************
 * @brief Set the state of an LED.
 *
 * This function sets the state of the specified LED (battery 
 * or status) to the given state (e.g., RED, GREEN, ORANGE).
 * 
 * @note Blinking led states are handled in LED timer ISR.
 *
 * @param led The type of LED (battery or status).
 * @param state The state to set the LED to (e.g., RED, GREEN, 
 *              ORANGE, BLINK_RED, BLINK_GREEN, BLINK_ORANGE).
 *************************************************************/
void Leds::set(ledType led, ledState state){
    LedBlinkTimer.disableTimer();   /* Enter critical section */
    _config[led].state = state;     /* Set LED-config */
    LedBlinkTimer.enableTimer();    /* Exit critical section */

    /* Select led action (if not blinking)*/
    switch(_config[led].state){
        case RED:
            digitalWrite(_config[led].pin_r, LOW);  
            digitalWrite(_config[led].pin_g, HIGH); 
        break;

        case GREEN:
            digitalWrite(_config[led].pin_r, HIGH);  
            digitalWrite(_config[led].pin_g, LOW); 
        break;

        case ORANGE:
            digitalWrite(_config[led].pin_r, LOW);  
            digitalWrite(_config[led].pin_g, LOW); 
        break;

        default:; /* Ignore this case. Blinking leds are covered within LED timer ISR */

    }
}

/************************************************************
 * @brief Timer callback function for LED blinking.
 *
 * This function is called by the timer interrupt to handle the blinking of LEDs.
 *
 * @param timerNo The timer number (unused).
 * @return Always returns true.
 *************************************************************/
bool IRAM_ATTR Leds::_callbackTimerLed(void * timerNo){
    static bool toggle[LED_COUNT] = {0};

    for(int i=0; i<LED_COUNT; i++){
        if(_config[i].state == BLINK_GREEN){
            digitalWrite(_config[i].pin_r, HIGH);  
            digitalWrite(_config[i].pin_g, toggle[i]); 
        }
        else if(_config[i].state == BLINK_RED){
            digitalWrite(_config[i].pin_r, toggle[i]);  
            digitalWrite(_config[i].pin_g, HIGH); 
        }
        else if(_config[i].state == BLINK_ORANGE){
            digitalWrite(_config[i].pin_r, toggle[i]);  
            digitalWrite(_config[i].pin_g, toggle[i]); 
        }
        else {} /* Ignore this case */
        
        toggle[i] = !toggle[i];
    }

    return true;
}



