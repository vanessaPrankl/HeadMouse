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

#ifdef IMU_BNO055_TEST_1
#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "pin_config_board_v1.h"
#include "BNO055_support.h"		//Contains the bridge code between the API and Arduino

//The device address is set to BNO055_I2C_ADDR1 in this example. You can change this in the BNO055.h file in the code segment shown below.
// /* BNO055 I2C Address */
// #define BNO055_I2C_ADDR1                0x28
// #define BNO055_I2C_ADDR2                0x29
// #define BNO055_I2C_ADDR                 BNO055_I2C_ADDR1

//Pin assignments as tested on the Arduino Due.
//Vdd,Vddio : 3.3V
//GND : GND
//SDA/SCL : SDA/SCL
//PSO/PS1 : GND/GND (I2C mode)

//This structure contains the details of the BNO055 device that is connected. (Updated after initialization)
struct bno055_t myBNO;

void setup() //This code is executed once
{

  //Initialize the Serial Port to view information on the Serial Monitor
  Serial.begin(115200);
  delay(2000);

  pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up

	//Initialize I2C communication
	if(Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL)){
    Serial.println("\nI2C ready!");
  }
  else{
    Serial.println("\nCannot start I2C...");
  }

	//Initialization of the BNO055
	if(BNO055_Zero_U8X  != BNO_Init(&myBNO)) //Assigning the structure to hold information about the device
  {
    Serial.println("\nBNO055 ready!");
  }
  else{
    Serial.println("\nCannot connect to BNO055");      
  }
  delay(1000);
	//Configuration to NDoF mode
	bno055_set_operation_mode(OPERATION_MODE_NDOF);

	delay(1000);

}

void loop() //This code is looped forever
{
  unsigned char chip_id = ' ';
  Serial.println("\n-----------------------------");

  //Read out device information
	Serial.print("Chip ID: ");
  if(BNO055_Zero_U8X == bno055_read_chip_id(&chip_id)){
      Serial.println("Cannot read chip ID");      
  }
  else{
    Serial.println(chip_id);
  }
	

	//Read out the software revision ID
	Serial.print("Software Revision ID: ");
	Serial.println(myBNO.sw_revision_id);

	//Read out the page ID
	Serial.print("Page ID: ");
	Serial.println(myBNO.page_id);

	//Read out the accelerometer revision ID
	Serial.print("Accelerometer Revision ID: ");
	Serial.println(myBNO.accel_revision_id);

	//Read out the gyroscope revision ID
	Serial.print("Gyroscope Revision ID: ");
	Serial.println(myBNO.gyro_revision_id);

	//Read out the magnetometer revision ID
	Serial.print("Magnetometer Revision ID: ");
	Serial.println(myBNO.mag_revision_id);

	//Read out the bootloader revision ID
	Serial.print("Bootloader Revision ID: ");
	Serial.println(myBNO.bootloader_revision_id);

	//Read out the device address
	Serial.print("Device Address: ");
	Serial.println(myBNO.dev_addr);

  delay(5000);
}

#endif