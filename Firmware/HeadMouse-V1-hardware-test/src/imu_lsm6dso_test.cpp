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

#ifdef IMU_LSM6DSO_TEST
#include <Arduino.h>
#include <Wire.h>
#include "SparkFunLSM6DSO.h"
#include "pin_config_board_v1.h"

LSM6DSO lsm6dso;


/* INIT *****************************************************************/
void setup() {

  Serial.begin(9600);

  /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */

  /* Init LSM6DSO IMU */
  if(lsm6dso.begin(0x6A, Wire)){
    Serial.println("LSM6DSO ready");
  }
  else{
    Serial.println("LSM6DSO not reachable");
  }

  if(lsm6dso.initialize(BASIC_SETTINGS)){
    Serial.println("LSM6DSO initialized");
  }
  else{
    Serial.println("Canno init LSM6DSO");
  }

  delay(5000); 

}


/* MAIN ******************************************************************/
void loop() {

  /* Read LSM6DSO IMU params */
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(lsm6dso.readFloatAccelX(), 3);
  Serial.print(" Y = ");
  Serial.println(lsm6dso.readFloatAccelY(), 3);
  Serial.print(" Z = ");
  Serial.println(lsm6dso.readFloatAccelZ(), 3);

  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(lsm6dso.readFloatGyroX(), 3);
  Serial.print(" Y = ");
  Serial.println(lsm6dso.readFloatGyroY(), 3);
  Serial.print(" Z = ");
  Serial.println(lsm6dso.readFloatGyroZ(), 3);

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees F = ");
  Serial.println(lsm6dso.readTempF(), 3);

  delay(1000);       
}



#endif