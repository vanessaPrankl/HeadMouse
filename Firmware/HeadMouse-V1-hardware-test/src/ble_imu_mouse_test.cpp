/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse functionality 
/*              between headmouse and a host system.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef BLE_IMU_MOUSE_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"
#include "mc6470_arduino.hpp"
#include "BleMouse.h"

ArduinoMC6470 mc6470(&Wire, MC6470_ACCEL_ADDRESS_GND);
BleMouse bleMouse("HeadMouse V1", "FH Technikum Wien", 100);


/* INIT *****************************************************************/
void setup() {

  Serial.begin(115200);
  bleMouse.begin();

   /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */

  delay(5000); 
  Serial.println("ONLINE!");

  /* Init IMU */
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

  /* Read accel data */
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

  /* Process Data */
    if((acc_data.x <= 15) && (acc_data.x >= -15)) acc_data.x = 0;
    if((acc_data.y <= 15) && (acc_data.y >= -15)) acc_data.y = 0;


  /* Move mouse cursor */
  if(bleMouse.isConnected()){
    Serial.println("Move mouse");
    bleMouse.move((unsigned char)(acc_data.y/10), (unsigned char)(acc_data.x/10),0);  
  }

  delay(100); 
        
}

#endif