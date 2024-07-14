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
    SENSITIVITY_0 = 0,
    SENSITIVITY_1 = 10,
    SENSITIVITY_2 = 20,
    SENSITIVITY_3 = 30,
    SENSITIVITY_4 = 40,
    SENSITIVITY_5 = 50,
    SENSITIVITY_6 = 60,
    SENSITIVITY_7 = 70,
    SENSITIVITY_8 = 80,
    SENSITIVITY_9 = 90
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
* @brief Struct to store active preferences of HeadMouse.
* @param mode Mouse control mode
* @param sensititvity Level of movement sensitivity
* @param btn_actions Array of [4] button actions
*************************************************************/
struct HmPreferences{
    devMode mode = ABSOLUTE;
    devSensitivity sensititvity = SENSITIVITY_4;
    btnAction btn_actions[4] = {NONE, NONE, NONE, NONE};
};