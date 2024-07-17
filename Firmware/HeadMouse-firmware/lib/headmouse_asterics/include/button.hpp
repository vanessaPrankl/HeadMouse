#pragma once

#include "def_general.hpp"
#include "def_preferences.hpp"

/*! *********************************************************
* @brief Enum to define possible mouse actions
*************************************************************/
enum actions {
    CLICK,
    PRESS,
    RELEASE
};

constexpr uint8_t BUTTON_COUNT = 4;
constexpr uint32_t BTN_DEBOUNCE_MS = 25;    // Debounce time until button push is valid
constexpr uint32_t BTN_TIMEOUT_COUNT = 250000/BTN_DEBOUNCE_MS;  // Timeout count for max button press duration to prevent overflow
constexpr uint32_t BTN_CLICK_MAX_COUNT = 500/BTN_DEBOUNCE_MS; // Count for maximum click time of button (longer button push is rated as press)


/*! *********************************************************
* @brief Class to handle interrupt based buttons
*************************************************************/
class Buttons {
private: 
    const pin _pins[BUTTON_COUNT]; // Array of uC pins which buttons are attached to
    uint32_t _count = 0;    // Internal counter for passed time since button was pushed

    static Buttons* instance; // Static instance pointer for singleton

    Buttons(pin pin0, pin pin1, pin pin2, pin pin3)    // Private constructor to prevent multiple instances
            : _pins{pin0, pin1, pin2, pin3} {}

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
    err initPins();
};