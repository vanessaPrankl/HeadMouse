#pragma once

#include "./include/def_general.hpp"
#include "./include/def_preferences.hpp"
#include "./include/def_status.hpp"
#include "./include/hm_board_config_v1_0.hpp"
#include "./include/led.hpp"
#include "./include/button.hpp"
#include "Adafruit_Sensor.h"

namespace _headmouse{
    extern volatile bool _measurement_available;
}

class HeadMouse {
    private:
    HmStatus _status;
    HmPreferences _preferences;
    Buttons* _buttons = Buttons::getInstance(PIN_BTN_1, PIN_BTN_2, PIN_BTN_3, PIN_BTN_4);
    Leds* _leds = Leds::getInstance(PIN_LED_BAT_G, PIN_LED_BAT_R, PIN_LED_STATUS_G, PIN_LED_STATUS_R);
    sensors_event_t _imu_data;

    void _initPins();
    void _initPreferences(HmPreferences);
    void _batStatusInterpreter();
    void _devStatusInterpreter();
    static bool _callbackTimerProgramCycle(void *);
   
    public:
    HeadMouse(){}
    bool isMeasurementAvailable();

    err init(HmPreferences);
    HmStatus updateDevStatus();
    err updateMovements();
    void updateBtnActions();

    void setPreferences(HmPreferences);
    void setSensitivity(devSensitivity);
    void setMode(devMode);
    void setButtonActions(btnAction*);

    void updateBatStatus();
    bool isCalibrated();
    bool isConnected();
    bool isCharging();
};