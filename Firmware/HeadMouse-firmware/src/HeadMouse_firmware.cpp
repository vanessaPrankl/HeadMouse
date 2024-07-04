/* HEADMOUSE ************************************************************/
/* 
/* Description: This is the firmware for HeadMouse V1.1.1
/* Author: Vanessa Prankl
/* Date: 20.06.2024
/* 
/************************************************************************/
 
#include <Arduino.h>
#include <Wire.h>
#include "hm_board_config_v1_0.hpp"
#include "default_preferences.hpp"
#include "logging.cpp"
#include "preferences.hpp"
#include "headmouse.hpp"

HeadMouse hm;

void displaySensorDetails(void);
void displayCalStatus(void);
void displaySensorStatus(void);

/* INIT *****************************************************************/
void setup() {
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

  hm.init(preferences);
 
}


/* MAIN ******************************************************************/
void loop() {
    hm.updateDevStatus();
    hm.updateMovements();
    //hm.updateBtnActions();
}
