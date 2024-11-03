#pragma once

#include "def_preferences.hpp"
#include "hm_board_config_v1_0.hpp"

using namespace preferences;

/* DEVICE CONFIG DEFAULTS ********************************************/
constexpr devMode HM_DEF_MODE = RELATIVE;
constexpr devSensitivity HM_DEF_SENSITIVITY = PREF_SENSITIVITY[4];
constexpr btnAction HM_DEF_ACTION_BTN_1 = LEFT;
constexpr btnAction HM_DEF_ACTION_BTN_2 = RIGHT;
constexpr btnAction HM_DEF_ACTION_BTN_3 = DEVICE_CONN_AND_CONFIG;
constexpr btnAction HM_DEF_ACTION_BTN_4 = SENSITIVITY;

