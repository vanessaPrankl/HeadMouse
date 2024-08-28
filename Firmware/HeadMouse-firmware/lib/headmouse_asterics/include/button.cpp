#include <Arduino.h>
#include "button.hpp"
#include "logging.hpp"
#include "def_general.hpp"
#include "hw_isr.hpp"

static constexpr uint32_t BTN_DEBOUNCE_MS = 25;    // Debounce time until button push is valid
static constexpr uint32_t BTN_TIMEOUT_COUNT = 250000/BTN_DEBOUNCE_MS;  // Timeout count for max button press duration to prevent overflow
static constexpr uint32_t BTN_CLICK_MAX_COUNT = 200/BTN_DEBOUNCE_MS; // Count for maximum click time of button (longer button push is rated as press)


namespace isr{
    ESP32Timer BtnTimer(0);
}
using namespace isr;


/* Define the static instance pointer */
Buttons* Buttons::instance = nullptr;

/************************************************************
 * @brief Get the singleton instance of the Buttons class.
 *
 * This function initializes and returns the singleton instance of
 * the Buttons class. If the instance does not already exist, it
 * creates a new one.
 *
 * @param pin_btn_0 The pin number for the first button.
 * @param pin_btn_1 The pin number for the second button.
 * @param pin_btn_2 The pin number for the third button.
 * @param pin_btn_3 The pin number for the fourth button.
 * @return A pointer to the singleton instance of the Buttons class.
 *************************************************************/
Buttons* Buttons::getInstance(pin pin_btn_0, pin pin_btn_1, pin pin_btn_2, pin pin_btn_3) {
    if (instance == nullptr) {
        instance = new Buttons(pin_btn_0, pin_btn_1, pin_btn_2, pin_btn_3);
    }
    return instance;
}

/************************************************************
 * @brief Initialize the button pins.
 *
 * This function sets up the button pins as input with pull-up
 * resistors and attaches a timer interrupt for debouncing.
 *
 * @return ERR_NONE if initialization is successful, otherwise
 * ERR_GENERIC.
 *************************************************************/
err Buttons::initPins(){
    err error = ERR_GENERIC;
    for(int i=0; i<BUTTON_COUNT; i++){
        pinMode(_pins[i], INPUT_PULLUP);
        log_message(LOG_DEBUG, "Pin %d initialized as input with pullup", i);
    }

    if (BtnTimer.attachInterruptInterval(BTN_DEBOUNCE_MS * 1000, callbackTimerBtn)){
        BtnTimer.stopTimer();
        error = ERR_NONE;
    }
    else error = ERR_GENERIC;
    
    return error;
}


/************************************************************
 * @brief Enable button interrupts.
 *
 * This function resets the button activity states and attaches
 * interrupts to the button pins to detect button presses.
 *************************************************************/
void Buttons::enableButtonInterrupts() {
    /* Reset button activity first */
    for(int i=0; i<BUTTON_COUNT; i++){
        is_active[i] = false;
    }

    /* Then start pin change interrupts */
    attachInterrupt(digitalPinToInterrupt(_pins[0]), callbackBtnPress0, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[1]), callbackBtnPress1, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[2]), callbackBtnPress2, FALLING);
    attachInterrupt(digitalPinToInterrupt(_pins[3]), callbackBtnPress3, FALLING);
   
}

/************************************************************
 * @brief Disable button interrupts.
 *
 * This function detaches interrupts from the button pins to stop
 * detecting button presses.
 *************************************************************/
void Buttons::disableButtonInterrupts() {
    for(int i=0; i<BUTTON_COUNT; i++){
        detachInterrupt(digitalPinToInterrupt(_pins[i]));
    }  
}


/************************************************************
 * @brief Handle a button press event.
 *
 * This function handles a button press event by disabling further
 * button interrupts and setting the button as active. It also
 * restarts the debounce timer.
 *
 * @param index The index of the button that was pressed.
 *************************************************************/
void Buttons::_handleBtnPress(int index) {
    disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
    is_active[index] = true;
    BtnTimer.restartTimer();    /* Start debounce mechanism */
}

/************************************************************
 * @brief Callback function for button-0 press interrupt.
 *
 * This function is called when the button is pressed and
 * handles the button press event.
 *************************************************************/
void IRAM_ATTR Buttons::callbackBtnPress0() {
    if (instance) {
        instance->_handleBtnPress(0);
    }
}

/************************************************************
 * @brief Callback function for button-1 press interrupt.
 *
 * This function is called when the button is pressed and
 * handles the button press event.
 *************************************************************/
void IRAM_ATTR Buttons::callbackBtnPress1() {
    if (instance) {
        instance->_handleBtnPress(1);
    }
}

/************************************************************
 * @brief Callback function for button-2 press interrupt.
 *
 * This function is called when the button is pressed and
 * handles the button press event.
 *************************************************************/
void IRAM_ATTR Buttons::callbackBtnPress2() {
    if (instance) {
        instance->_handleBtnPress(2);
    }
}


/************************************************************
 * @brief Callback function for the button-3 press interrupt.
 *
 * This function is called when the button is pressed and
 * handles the button press event.
 *************************************************************/
void IRAM_ATTR Buttons::callbackBtnPress3() {
    if (instance) {
        instance->_handleBtnPress(3);
    }
}

/************************************************************
 * @brief Timer callback function for button debounce handling.
 *
 * This function is called by the timer interrupt to handle
 * debouncing of the button presses. It detects if a button is still
 * pressed and handles click and press events accordingly.
 *
 * @param timerNo The timer number (unused).
 * @return Always returns true.
 *************************************************************/
bool IRAM_ATTR Buttons::callbackTimerBtn(void* timerNo) {
    int index = 0;

    /* Detect which button is active */
    for (int i = 0; i < BUTTON_COUNT; i++) {
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