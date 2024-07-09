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
  preferences.buttons[0].pin = PIN_BTN_1;
  preferences.buttons[1].pin = PIN_BTN_2;
  preferences.buttons[2].pin = PIN_BTN_3;
  preferences.buttons[3].pin = PIN_BTN_4;
  preferences.buttons[0].action = HM_DEF_ACTION_BTN_1;
  preferences.buttons[1].action = HM_DEF_ACTION_BTN_2;
  preferences.buttons[2].action = HM_DEF_ACTION_BTN_3;
  preferences.buttons[3].action = HM_DEF_ACTION_BTN_4;

  log_init_serial();
  log_message(LOG_INFO, "Headmouse V1 - Serial interface up and running");

  error = hm.init(preferences);
  if(error == ERR_NONE)  log_message(LOG_INFO, "Setup done");
  else{
    log_message(LOG_ERROR, "Setup failed, error code: %d\n...", error);
  }
 
}


/* MAIN ******************************************************************/
void loop() {
    
    hm.updateDevStatus();
    sleep(1);    
    //hm.updateMovements();
    //hm.updateBtnActions();
}
