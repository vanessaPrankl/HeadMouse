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

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
  displaySensorStatus();
 
}


/* MAIN ******************************************************************/
void loop() {
  

}

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  bno.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" xxx");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" xxx");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display some basic info about the sensor status
*/
/**************************************************************************/
void displaySensorStatus(void)
{
  /* Get the system status values (mostly for debugging purposes) */
  uint8_t system_status, self_test_results, system_error;
  system_status = self_test_results = system_error = 0;
  bno.getSystemStatus(&system_status, &self_test_results, &system_error);

  /* Display the results in the Serial Monitor */
  Serial.println("");
  Serial.print("System Status: 0x");
  Serial.println(system_status, HEX);
  Serial.print("Self Test:     0b");
  Serial.println(self_test_results, BIN);
  Serial.print("System Error:  0x");
  Serial.println(system_error, HEX);
  Serial.println("");
  delay(500);
}

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}

#endif