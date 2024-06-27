#pragma once

#include "./include/status.hpp"
#include "./include/actions.hpp"
#include "./include/preferences.hpp"
#include "./include/error.hpp"
#include "./include/led.hpp"
#include "Adafruit_Sensor.h"


typedef class HeadMouse {
    private:
    /* Attributes */
    HmStatus _status;
    HmPreferences _preferences;
    ledState _led_bat = RED;
    ledState _led_status = BLINK_RED;
    sensors_event_t _imu_data;

    /* Methods */
    err _updateMovements();
    void _updateBtnActions();  
    void _updateLeds();
    void _initPins();
   
    public:
    /* Constructors */
    HeadMouse(){}

    /* Methods */
    err init(HmPreferences);
    err update();
    err pairNewDevice();
    err switchPairedDevice();

    /* Getter */
    HmStatus getDevStatus(err*);
    BatStatus getBatStatus(err*);
    bool isCalibrated();
    bool isConnected();
    bool isCharging();

    /* Setter */
    err setPreferences(HmPreferences);
    void setSensitivity(devSensitivity);
    void setMode(devMode);
    err setButtonAction(pin, btnAction);
};
