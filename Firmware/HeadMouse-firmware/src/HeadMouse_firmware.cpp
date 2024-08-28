/* HEADMOUSE ************************************************************/
/* 
/* Description: This is the firmware for HeadMouse V1.1.1
/* Author: Vanessa Prankl
/* Date: 20.06.2024
/* 
/************************************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include "default_preferences.hpp"
#include "logging.hpp"
#include "headmouse.hpp"

HeadMouse hm;

/* INIT *****************************************************************/
void setup() {
  err error = ERR_GENERIC;
  HmPreferences preferences;
  preferences.mode = HM_DEF_MODE;
  preferences.sensititvity = HM_DEF_SENSITIVITY;
  preferences.btn_actions[0] = HM_DEF_ACTION_BTN_1;
  preferences.btn_actions[1] = HM_DEF_ACTION_BTN_2;
  preferences.btn_actions[2] = HM_DEF_ACTION_BTN_3;
  preferences.btn_actions[3] = HM_DEF_ACTION_BTN_4;

#ifdef LOG_OVER_SERIAL
  log_init_serial();
#endif
  log_message(LOG_INFO, "Headmouse V1 - Serial interface up and running");

  log_message(LOG_INFO, "Starting setup...");
  error = hm.init(preferences);
  if(error == ERR_NONE)  log_message(LOG_INFO, "Setup done");
  else{
    log_message(LOG_ERROR, "Setup failed, error code: %d\n...", error);
  }
 
}


/* MAIN ******************************************************************/
void loop() {
    hm.updateDevStatus();
    hm.updateMovements();
    hm.updateBtnActions();
    //delayMicroseconds(100000);    /* Debug only */
}
