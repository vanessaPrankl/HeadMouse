/* HARDWARE TEST HEADMOUSE V1 *******************************************/
/* 
/* Description: This is an IMU test firmware for HeadMouse board version 1.
/* Author: Vanessa Prankl
/* Date: 06.02.202
/*
/* IMUs under test: MC6470, LSM6DSO, BNO055
/* 
/************************************************************************/
#include "program_selector.h"

#ifdef IMU_BNO055_TEST
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"

LSM6DSO lsm6dso;



/* INIT *****************************************************************/
void setup() {

  Serial.begin(9600);

  /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */


  delay(5000); 
}


/* MAIN ******************************************************************/
void loop() {

  delay(1000);       
}


#endif