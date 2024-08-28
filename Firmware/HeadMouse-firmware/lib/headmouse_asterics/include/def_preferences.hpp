#pragma once
#include <stdint.h>
#include "BleMouse.h"

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
    SENSITIVITY_1 = 5,
    SENSITIVITY_2 = 10,
    SENSITIVITY_3 = 15,
    SENSITIVITY_4 = 20,
    SENSITIVITY_5 = 25,
    SENSITIVITY_6 = 30,
    SENSITIVITY_7 = 35,
    SENSITIVITY_8 = 40,
    SENSITIVITY_9 = 45
};

/*! *********************************************************
* @brief Enum to define available button actions
*************************************************************/
enum btnAction {
    NONE,
    LEFT = MOUSE_LEFT,
    RIGHT = MOUSE_RIGHT,
    CONN_NEW_DEVICE,
    SENSITIVITY
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