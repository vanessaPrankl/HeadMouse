#pragma once

#include "preferences.hpp"
#include "hm_board_config_v1_0.hpp"

/* LOG-LEVELS ***************************************************/
#define LOG_LEVEL_DEBUG     1
#define LOG_LEVEL_DEBUG_BAT 1
#define LOG_LEVEL_DEBUG_IMU 1
#define LOG_LEVEL_INFO      1
#define LOG_LEVEL_WARNING   1
#define LOG_LEVEL_ERROR     1

/* DEVICE CONFIG DEFAULTS ********************************************/
constexpr devMode HM_DEF_MODE = RELATIVE;
constexpr devSensitivity HM_DEF_SENSITIVITY = SENSITIVITY_4;
constexpr btnAction HM_DEF_ACTION_BTN_1 = MOUSE_RIGHT;
constexpr btnAction HM_DEF_ACTION_BTN_2 = MOUSE_LEFT;
constexpr btnAction HM_DEF_ACTION_BTN_3 = MOUSE_WHEEL;
constexpr btnAction HM_DEF_ACTION_BTN_4 = BLE_CONNECTION;

