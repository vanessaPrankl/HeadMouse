#pragma once

#include "../include/status.hpp"
#include "../include/actions.hpp"
#include "../include/preferences.hpp"
#include "../include/error.hpp"
#include "../include/led.hpp"


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
    err init();
    err update();
    err pairNewDevice();
    err switchPairedDevice();

    /* Getter */
    HmStatus getStatus(err*);
    batStatus getBatStatus(err*);
    bool isCalibrated(err*);
    bool isConnected(err*);
    bool isCharging();

    /* Setter */
    err setSensitivity(devSensitivity);
    err setMode(devMode);
    err setButtonAction(uint8_t, btnAction);
};
