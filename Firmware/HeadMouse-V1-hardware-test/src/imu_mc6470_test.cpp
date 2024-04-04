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

#ifdef IMU_MC6470_TEST
#include <Arduino.h>
#include <Wire.h>
#include "mc6470_arduino.hpp"
#include "pin_config_board_v1.h"

ArduinoMC6470 mc6470(&Wire, MC6470_ACCEL_ADDRESS_GND);

void blinkLED(){
  digitalWrite(PIN_LED_BAT_R, HIGH);  
  digitalWrite(PIN_LED_BAT_G, LOW); 
  delay(200);
  digitalWrite(PIN_LED_BAT_R, HIGH);  
  digitalWrite(PIN_LED_BAT_G, HIGH); 
}

/* INIT *****************************************************************/
void setup() {
  /* Init LEDs */
  pinMode(PIN_LED_BAT_R, OUTPUT);
  pinMode(PIN_LED_BAT_G, OUTPUT);
   
  Serial.begin(9600);

  /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */

  delay(5000); 
  Serial.println("ONLINE!");
  uint32_t err = mc6470.begin();
  if(err != MC6470_Status_OK){
    Serial.println("ERR - MC6470 not reachable");
  }else{
    Serial.println("MC6470 ready");
  }
}


/* MAIN ******************************************************************/
void loop() {
  uint32_t err = MC6470_Status_OK;
  MC6470_MagReading mag_data;
  MC6470_AccelReading acc_data;
  static int i = 0;

  /* Signal still alive */
  blinkLED();

  /* Read temperature every 5 cycles */
  i++;
  if(i >= 5){
    int8_t temp = 0;
    err = mc6470.getTemp(&temp);
    if(err != MC6470_Status_OK){
      Serial.println("ERR - cannot read data");
    }else{
      Serial.println("Temp: ");
      Serial.print(temp);
    }
    i=0;
  }
  
  /* Read magnetometer and accelerometer */

  /* Read MC6470 IMU params */
  err = mc6470.getData(mag_data, acc_data);
  if(err != MC6470_Status_OK){
    Serial.println("ERR - cannot read data");
  }

  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(acc_data.x);
  Serial.print(" Y = ");
  Serial.println(acc_data.y);
  Serial.print(" Z = ");
  Serial.println(acc_data.z);

  Serial.print("\nMagnetometer:\n");
  Serial.print(" X = ");
  Serial.println(mag_data.x);
  Serial.print(" Y = ");
  Serial.println(mag_data.y);
  Serial.print(" Z = ");
  Serial.println(mag_data.z);

  delay(1000);       
}

#endif