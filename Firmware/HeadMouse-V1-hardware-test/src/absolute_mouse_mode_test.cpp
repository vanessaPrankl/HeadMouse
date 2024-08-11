/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse in absolute cursor 
/*              position mode.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef ABSOLUTE_MOUSE_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"
#include "BleMouse.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (50)

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28, &Wire);

BleMouse bleMouse("HeadMouse V1", "FH Technikum Wien", 100);

void displaySensorDetails(void);
void displayCalStatus(void);
void displaySensorStatus(void);

/* INIT *****************************************************************/
void setup() {

  Serial.begin(115200);
  while (!Serial) delay(10);  // wait for serial port to open!

  Serial.println("Orientation Sensor Test"); 

  bleMouse.begin();

  pinMode(PIN_I2C_SDA, INPUT); // Disable internal pull-up
  pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up
  Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL);

  /* Initialise the sensor */
  if(bno.begin())
  {
    Serial.print("BNO055 ready!");
  }
  else{
    Serial.print("Cannot connect to BNO05\n");      
  }
  delay(1000);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Optional: Display current status */
  displaySensorStatus();
 
}


/* MAIN ******************************************************************/
void loop() {
  static int first_run = true;
  int move_mouse_y = 0;
  int move_mouse_x = 0;
  static sensors_event_t event_buf;

  sensors_event_t event;

  /* Get a new sensor event */
  bno.getEvent(&event);
  if(first_run){
    first_run = false;
    event_buf.orientation.x = event.orientation.x;
    event_buf.orientation.y = event.orientation.y;
    event_buf.orientation.z = event.orientation.z;
  }

  /* Display the floating point data 
  Serial.print("\nX: ");
  Serial.print(event.orientation.x, 4);
  Serial.print("\tY: ");
  Serial.print(event.orientation.y, 4);
  Serial.print("\tZ: ");
  Serial.print(event.orientation.z, 4);*/

  //displayCalStatus();

  /* Process data */
  move_mouse_x = (int)(50*event.orientation.x) - (int)(50*event_buf.orientation.x);
  move_mouse_y = (int)(50*event_buf.orientation.y) - (int)(50*event.orientation.y);

  /* Move mouse cursor */
  if(bleMouse.isConnected()){
    bleMouse.move((unsigned char)(move_mouse_x), (unsigned char)(move_mouse_y),0);  
  }

  /* Store orientation values into buffer for later on comparison */
  event_buf.orientation.x = event.orientation.x;
  event_buf.orientation.y = event.orientation.y;
  //event_buf.orientation.z = event.orientation.z;

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