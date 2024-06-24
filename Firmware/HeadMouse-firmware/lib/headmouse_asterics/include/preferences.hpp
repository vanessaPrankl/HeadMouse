#pragma once
#include <stdint.h>

enum devMode {
    ABSOLUTE,
    RELATIVE
};

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

enum btnAction {
    MOUSE_RIGHT,
    MOUSE_LEFT,
    MOUSE_WHEEL,
    BLE_CONNECTION,
    SENSITIVITY,
    NONE
};

struct button {
    uint8_t pin = 0;
    btnAction action = NONE;
};

struct HmPreferences{
    devMode mode = ABSOLUTE;
    devSensitivity sensititvity = SENSITIVITY_4;
    button buttons[4];
};