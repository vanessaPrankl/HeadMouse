#pragma once
#include <stdint.h>

/*! *********************************************************
* @brief Enum to define available mouse movement modes
*************************************************************/
enum devMode {
    ABSOLUTE,
    RELATIVE
};

/*! *********************************************************
* @brief Enum to define available sensitivity levels
*************************************************************/
enum devSensitivity {
    SENSITIVITY_0,
    SENSITIVITY_1,
    SENSITIVITY_2,
    SENSITIVITY_3,
    SENSITIVITY_4,
    SENSITIVITY_5,
    SENSITIVITY_6,
    SENSITIVITY_7,
    SENSITIVITY_8,
    SENSITIVITY_9
};

/*! *********************************************************
* @brief Enum to define available button actions
*************************************************************/
enum btnAction {
    MOUSE_RIGHT,
    MOUSE_LEFT,
    MOUSE_WHEEL,
    BLE_CONNECTION,
    SENSITIVITY,
    NONE
};

/*! *********************************************************
* @brief Struct to store button pins on uC and associated actions
* @param pin uC pin which button is attached to
* @param action Device action associated with button
*************************************************************/
struct button {
    uint8_t pin = 0;
    btnAction action = NONE;
};

/*! *********************************************************
* @brief Struct to store active preferences of HeadMouse.
* @param mode Mouse control mode
* @param sensititvity Level of movement sensitivity
* @param buttons Array of [4] buttons with associated actions
*************************************************************/
struct HmPreferences{
    devMode mode = ABSOLUTE;
    devSensitivity sensititvity = SENSITIVITY_4;
    button buttons[4];
};