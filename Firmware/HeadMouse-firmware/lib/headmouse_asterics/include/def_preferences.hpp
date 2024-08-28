#pragma once
#include <stdint.h>
#include "BleMouse.h"

namespace preferences{
    /*! *********************************************************
    * @brief Type to define sensitivity levels
    *************************************************************/
    typedef uint32_t devSensitivity;

    constexpr char* STORE_MODE = "mode";
    constexpr char* STORE_SENSITIVITY = "sensitivity";
    constexpr char* STORE_BTN[4] = {"button0", "button1", "button2", "button3"};
    constexpr devSensitivity SENSITIVITY_STEP = 10;
    constexpr devSensitivity SENSITIVITY_MIN = 0;
    constexpr devSensitivity SENSITIVITY_MAX = SENSITIVITY_STEP * 4;
    constexpr devSensitivity PREF_SENSITIVITY[5] = {SENSITIVITY_MIN, SENSITIVITY_STEP*1, SENSITIVITY_STEP*2, SENSITIVITY_STEP*3, SENSITIVITY_STEP*4};
}

using namespace preferences;

/*! *********************************************************
* @brief Enum to define available mouse movement modes
*************************************************************/
enum devMode {
    ABSOLUTE,
    RELATIVE
};


/*! *********************************************************
* @brief Enum to define available button actions
*************************************************************/
enum btnAction {
    NONE,
    LEFT = MOUSE_LEFT,
    RIGHT = MOUSE_RIGHT,
    SENSITIVITY,
    DEVICE_CONN_AND_CONFIG
};

/*! *********************************************************
* @brief Struct to store active preferences of HeadMouse.
* @param mode Mouse control mode
* @param sensititvity Level of movement sensitivity
* @param btn_actions Array of [4] button actions
*************************************************************/
struct HmPreferences{
    devMode mode = ABSOLUTE;
    devSensitivity sensititvity = PREF_SENSITIVITY[4];
    btnAction btn_actions[4] = {NONE, NONE, NONE, NONE};
};