/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse functionality 
/*              between headmouse and a host system.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef ABSOLUTE_MOUSE_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"
#include "mc6470_arduino.hpp"
#include "BleMouse.h"

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
 
}


/* MAIN ******************************************************************/
void loop() {
  uint32_t err = MC6470_Status_OK;
  static int i = 0;

  /* Read data */
 

  /* Process data */
    

  /* Move mouse cursor */
  if(bleMouse.isConnected()){
    Serial.println("Move mouse");
    bleMouse.move((unsigned char)(acc_data.y/10), (unsigned char)(acc_data.x/10),0);  
  }

  delay(100); 
        
}

#endif