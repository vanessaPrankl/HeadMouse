/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse functionality 
/*              between headmouse and a host system.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef BLE_MOUSE_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"
#include "BleMouse.h"

BleMouse bleMouse("HeadMouse V1", "FH Technikum Wien", 100);


/* INIT *****************************************************************/
void setup() {

  Serial.begin(115200);
  bleMouse.begin();
}


/* MAIN ******************************************************************/
void loop() {
  
  /* Move mouse cursor */
  Serial.println("Move mouse");
  for(int i=0; i<10; i++){
    if(bleMouse.isConnected()){
      bleMouse.move(1,1,0);
    }
    delay(100); 
  }

  /* Scroll down */
  Serial.println("Scroll down");
  for(int i=0; i<10; i++){
    if(bleMouse.isConnected()){
      bleMouse.move(0,0,-1);
    }
    delay(100); 
  }
  
  /* Mouse clicks */
    Serial.println("Right click");
    bleMouse.click(MOUSE_RIGHT);
    delay(1000);

    Serial.println("Scroll wheel click");
    bleMouse.click(MOUSE_MIDDLE);
    delay(1000);

    Serial.println("Back button click");
    bleMouse.click(MOUSE_BACK);
    delay(1000);

    Serial.println("Forward button click");
    bleMouse.click(MOUSE_FORWARD);
    delay(1000);

    Serial.println("Click left+right mouse button at the same time");
    bleMouse.click(MOUSE_LEFT | MOUSE_RIGHT);
    delay(1000);

    Serial.println("Click left+right mouse button and scroll wheel at the same time");
    bleMouse.click(MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE);
    delay(1000);
        
}

#endif