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

    constexpr int SCALING_FACTOR = 1000;   // Used to bring calculations from float to int with necessary accuracy
    constexpr int JITTER_OFFSET = (int)(0.02*SCALING_FACTOR);       // [RAD]*scaling factor
    constexpr int SLOW_MOTION_OFFSET = (int)(0.07*SCALING_FACTOR);   // [RAD]*scaling factor
    constexpr devSensitivity SENSITIVITY_STEP = 7;
    constexpr devSensitivity SENSITIVITY_STEP_COUNT = 5;
    constexpr devSensitivity SENSITIVITY_MIN = 20;
    constexpr devSensitivity SENSITIVITY_MAX = SENSITIVITY_MIN + SENSITIVITY_STEP * (SENSITIVITY_STEP_COUNT-1);
    constexpr devSensitivity PREF_SENSITIVITY[SENSITIVITY_STEP_COUNT] = {SENSITIVITY_MIN, SENSITIVITY_MIN+SENSITIVITY_STEP, SENSITIVITY_MIN+SENSITIVITY_STEP*2, SENSITIVITY_MIN+SENSITIVITY_STEP*3, SENSITIVITY_MAX};

    constexpr int SLOWMO_ANGLE_DEFLECTION[6] = {
                                                20,
                                                30,
                                                40,
                                                50,
                                                60,
                                                70
                                                };
    constexpr int SLOWMO_SENSITIVITY[5][5] = {
                                                {20, 20, 20, 20, 20},
                                                {20, 21, 22, 23, 24},
                                                {20, 23, 25, 27, 28},
                                                {20, 24, 27, 31, 34},
                                                {20, 25, 31, 36, 40}
                                                };
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