/* CIRCUIT BOARD V1.0 PIN CONFIG *********************************************/
/* 
/* Description: This is a hardware pin config file for HeadMouse board version 1.
/* Author: Vanessa Prankl
/* Date: 26.03.2024
/* 
/************************************************************************/
#pragma once
#include <Arduino.h>

typedef uint8_t pin;

/* LEDs */
constexpr pin PIN_LED_BAT_R = 6;
constexpr pin PIN_LED_BAT_G = 7;
constexpr pin PIN_LED_STATUS_R = 17;
constexpr pin PIN_LED_STATUS_G = 18;

/* Buttons */
constexpr pin PIN_BTN_1 = 42;
constexpr pin PIN_BTN_2 = 41;
constexpr pin PIN_BTN_3 = 40;
constexpr pin PIN_BTN_4 = 39;

/* Battery */
constexpr pin PIN_VBATT_MEASURE = 4;     /* To measure current battery voltage */
constexpr pin PIN_BATT_STATUS = 5;       /* Pulled low if charging is active*/

/* I2C */
constexpr pin PIN_I2C_SCL = 9;
constexpr pin PIN_I2C_SDA = 10;

/* IMU interrupt pins */
constexpr pin PIN_LSM6DS_INT = 11;
constexpr pin PIN_MC6470_INT = 12;
constexpr pin PIN_BNO55_INT = 13;

/* BNO055 config */
constexpr int32_t BNO055_SENSOR_ID = 55;
constexpr uint8_t BNO055_I2C_ADDRESS = 0x28;

/* Serial communication config */
constexpr uint32_t SERIAL_BAUD_RATE = 115200;

/* BLE Device config */
constexpr char DEVICE_NAME[] = "HeadMouse V1";
constexpr char DEVICE_MANUFACTURER[] = "FH Technikum Wien";
constexpr uint8_t BAT_LEVEL_DUMMY = 100;
