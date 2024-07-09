#pragma once

#include "./include/status.hpp"
#include "./include/actions.hpp"
#include "./include/preferences.hpp"
#include "./include/error.hpp"
#include "./include/led.hpp"
#include "./include/hm_board_config_v1_0.hpp"
#include "Adafruit_Sensor.h"

class HeadMouse {
    private:
    /* Attributes */
    HmStatus _status;
    HmPreferences _preferences;
    ledState _led_bat = RED;
    ledState _led_status = BLINK_RED;
    sensors_event_t _imu_data;

    /* Methods */
    void _initPins();
    void _setLed(ledType, ledState);
    void _batStatusInterpreter();
    void _devStatusInterpreter();
   
    public:
    /* Constructors */
    HeadMouse(){}

    /* Methods */
    err init(HmPreferences);
    HmStatus updateDevStatus();
    err updateMovements();
    void updateBtnActions();
    err pairNewDevice();
    err switchPairedDevice();

    /* Setter */
    err setPreferences(HmPreferences);
    void setSensitivity(devSensitivity);
    void setMode(devMode);
    err setButtonAction(pin, btnAction);

    /* Getter */
    BatStatus getBatStatus();
    bool isCalibrated();
    bool isConnected();
    bool isCharging();
};
