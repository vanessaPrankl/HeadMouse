/* HARDWARE TEST HEADMOUSE V1 *******************************************/
/* 
/* Description: This is a hardware test firmware for HeadMouse board version 1.
/* Author: Vanessa Prankl
/* Date: 11.01.2024
/* 
/************************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include "SparkFunLSM6DSO.h"

/* PIN DEFINITIONS ******************************************************/

const int8_t PIN_I2C_SCL = 9;
const int8_t PIN_I2C_SDA = 10;
const int8_t PIN_LSM6DS_INT = 11;
const int8_t PIN_MC6470_INT = 12;
const int8_t PIN_BNO55_INT = 13;

/* FUNCTION DEFINITIONS **************************************************/
void i2c_bus_scan(){
  byte error, address;
  int nDevices;
  Serial.println("\n=== I2C Scanner ===");
  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("Device at address: 0x");
      if (address < 16) Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("");
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknown error at address: 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) Serial.println("No I2C devices found\n");
  else Serial.println("I2C scan finished\n");
}


/* INIT *****************************************************************/
void setup() {

  Serial.begin(9600);

  /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);

  delay(5000); 
}

/* MAIN ******************************************************************/
void loop() {

  i2c_bus_scan();

  delay(1000);       
}


