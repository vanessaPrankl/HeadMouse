#pragma once

#include "../include/status.hpp"
#include "../include/actions.hpp"
#include "../include/preferences.hpp"
#include "../include/error.hpp"
#include "../include/led.hpp"


typedef class HeadMouse {
    private:
    HmStatus status;
    HmPreferences preferences;
    ledState led_bat = RED;
    ledState led_status = BLINK_RED;
    err updateMovements();
    err updateBtnActions();
   
    public:
    err init();
    err update();
    err pairNewDevice();
    err switchPairedDevice();

    /* Getter */
    HmStatus get_status(err*);
    batStatus getBatStatus(err*);
    bool isCalibrated(err*);
    bool isConnected(err*);
    bool isCharging();

    /* Setter */
    err setSensitivity(devSensitivity);
    err setMode(devMode);
    err setButtonAction(uint8_t, btnAction);
};
