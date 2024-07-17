#include <Arduino.h>
#include "led.hpp"
#include "logging.hpp"
#include "hw_isr.hpp"
#include "def_general.hpp"

using namespace isr;

// Define the static instance pointer
Leds* Leds::instance = nullptr;

Leds* Leds::getInstance(pin pin_status_g, pin pin_status_r, pin pin_battery_g, pin pin_battery_r) {
    if (instance == nullptr) {
        instance = new Leds(pin_status_g, pin_status_r, pin_battery_g, pin_battery_r);
    }
    return instance;
}

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

/*! *********************************************************
* @brief Set led action 
* @param led_type Battery or status led
* @param led_state Led action type 
* @return None
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

        default: /* Ignore this case. Blinking leds are covered within LED timer ISR */

    }
}

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



