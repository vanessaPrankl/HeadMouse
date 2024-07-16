#pragma once

#include "./actions.hpp"
#include "./hm_board_config_v1_0.hpp"
#include "./preferences.hpp"

/*! *********************************************************
* @brief Class to handle interrupt based buttons
*************************************************************/
class Buttons {
private: 
    const pin _pins[BUTTON_COUNT]; // Array of uC pins which buttons are attached to
    uint32_t _count = 0;    // Internal counter for passed time since button was pushed

    static Buttons* instance; // Static instance pointer for singleton

    Buttons(pin pin0, pin pin1, pin pin2, pin pin3);    // Private constructor to prevent multiple instances

    static void IRAM_ATTR callbackBtnPress0();
    static void IRAM_ATTR callbackBtnPress1();
    static void IRAM_ATTR callbackBtnPress2();
    static void IRAM_ATTR callbackBtnPress3();

    static bool IRAM_ATTR callbackTimerBtn(void* timerNo);

    void _handleBtnPress(int index);

public:
    btnAction actions[BUTTON_COUNT] = {NONE};   // Array of device actions associated with buttons
    bool is_active[BUTTON_COUNT] = {false};     // TRUE if button is currently active (resulting action 
                                                // (click/press/invalid) has not been detected yet)
    bool is_click[BUTTON_COUNT] = {false};      // TRUE if button click recently detected
    bool is_press[BUTTON_COUNT] = {false};      // TRUE if button press recently detected

    // Static method to get the singleton instance
    static Buttons* getInstance(pin pin0, pin pin1, pin pin2, pin pin3);

    void enableButtonInterrupts();
    void disableButtonInterrupts();
    void initPins();
};