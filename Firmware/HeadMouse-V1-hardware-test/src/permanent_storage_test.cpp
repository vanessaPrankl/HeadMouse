/* PERMANENT STORAGE TEST *******************************************/
/* 
/* Description: This is a permanent storage test to store preferences on ESP32-S3.
/* Author: Vanessa Prankl
/* Date:26.08.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef PERMANENT_STORAGE_TEST 
#include <Arduino.h>
#include <Preferences.h>
#include "pin_config_board_v1.h"

Preferences nonVolatileMemory;

/* FUNCTION DEFINITIONS **************************************************/

/* INIT *****************************************************************/
void setup() {

  Serial.begin(9600);
  nonVolatileMemory.begin("device_config", false);  // Open preferences namespace in read/write mode
  
  delay(3000);
  Serial.println("Started non volatile preferences test");
}

/* MAIN ******************************************************************/
void loop() {
  uint32_t sensitivity = nonVolatileMemory.getUInt("sensitivity", 0);

  Serial.print("\nsensitivity value is: ");
  Serial.print(sensitivity);
  sensitivity++;
  nonVolatileMemory.putUInt("sensitivity", sensitivity);
  
  Serial.print("\nRestarting ESP...");
  ESP.restart();
  delay(1000);       
}
#endif

