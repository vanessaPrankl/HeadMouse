#include <Arduino.h>
#include "button.hpp"
#include "logging.hpp"
#include "def_general.hpp"
#include "hw_isr.hpp"


namespace isr{
    ESP32Timer BtnTimer(0);
}
using namespace isr;


// Define the static instance pointer
Buttons* Buttons::instance = nullptr;

Buttons* Buttons::getInstance(pin pin0, pin pin1, pin pin2, pin pin3) {
    if (instance == nullptr) {
        instance = new Buttons(pin0, pin1, pin2, pin3);
    }
    return instance;
}

err Buttons::initPins(){
    err error = ERR_GENERIC;
    for(int i=0; i++; i<BUTTON_COUNT){
        pinMode(_pins[i], INPUT_PULLUP);
    }

    if (BtnTimer.attachInterruptInterval(BTN_DEBOUNCE_MS * 1000, callbackTimerBtn)){
        error = ERR_NONE;
    }
    else error = ERR_GENERIC;
    
    return error;
}

void Buttons::enableButtonInterrupts() {
    /* Reset button activity first */
    for(int i=0; i++; i<BUTTON_COUNT){
        is_active[i] = false;
    }

    /* Then start pin change interrupts */
    attachInterrupt(digitalPinToInterrupt(_pins[0]), callbackBtnPress0, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[1]), callbackBtnPress1, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[2]), callbackBtnPress2, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[3]), callbackBtnPress3, FALLING);
}

void Buttons::disableButtonInterrupts() {
    for(int i=0; i++; i<BUTTON_COUNT){
        detachInterrupt(digitalPinToInterrupt(_pins[i]));
    }  
}

void Buttons::_handleBtnPress(int index) {
    disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
    is_active[index] = true;
    BtnTimer.restartTimer();
}

void IRAM_ATTR Buttons::callbackBtnPress0() {
    if (instance) {
        instance->_handleBtnPress(0);
    }
}

void IRAM_ATTR Buttons::callbackBtnPress1() {
    if (instance) {
        instance->_handleBtnPress(1);
    }
}

void IRAM_ATTR Buttons::callbackBtnPress2() {
    if (instance) {
        instance->_handleBtnPress(2);
    }
}

void IRAM_ATTR Buttons::callbackBtnPress3() {
    if (instance) {
        instance->_handleBtnPress(3);
    }
}

bool IRAM_ATTR Buttons::callbackTimerBtn(void* timerNo) {
    int index = 0;

    /* Detect which button is active */
    for (int i = 0; i < BUTTON_COUNT; ++i) {
        if (instance->is_active[i]) {
            index = i;
            break;
        }
    }

    /* Button still pressed? */
    if (!digitalRead(instance->_pins[index])) {
        instance->_count++;
        if ((instance->_count >= BTN_CLICK_MAX_COUNT) && (!instance->is_press[index])) {
            instance->is_press[index] = true;  /* Detect button press if it has been pressed for at least 500ms */
        }
        if (instance->_count >= BTN_TIMEOUT_COUNT) { /* Timeout to prevent overflow */
            instance->_count = 0;
            instance->is_press[index] = false;
            BtnTimer.stopTimer();
            instance->enableButtonInterrupts();
        }
    } /* Button not pressed any more */
    else if (instance->_count == 0) {
        /* Ignore this case, invalid click/press duration */
        BtnTimer.stopTimer();
        instance->enableButtonInterrupts();
    } else if (instance->_count < BTN_CLICK_MAX_COUNT) { /* Only detect click if 25-500ms have passed since button release */
        instance->is_click[index] = true;
        instance->_count = 0;
        BtnTimer.stopTimer();
        instance->enableButtonInterrupts();
    } else { /* Complete button press */
        instance->is_press[index] = false;
        instance->_count = 0;
        BtnTimer.stopTimer();
        instance->enableButtonInterrupts();
    }

    return true;
}