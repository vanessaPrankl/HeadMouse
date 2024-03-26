/* CIRCUIT BOARD V1 PIN CONFIG *********************************************/
/* 
/* Description: This is a hardware pin config file for HeadMouse board version 1.
/* Author: Vanessa Prankl
/* Date: 26.03.2024
/* 
/************************************************************************/
#pragma once
#include <Arduino.h>

/* LEDs */
const int8_t PIN_LED_BAT_R = 6;
const int8_t PIN_LED_BAT_G = 7;
const int8_t PIN_LED_STATUS_R = 17;
const int8_t PIN_LED_STATUS_G = 18;

/* Buttons */
const int8_t PIN_BTN_1 = 42;
const int8_t PIN_BTN_2 = 41;
const int8_t PIN_BTN_3 = 40;
const int8_t PIN_BTN_4 = 39;

/* Battery */
const int8_t PIN_VBATT_MEASURE = 4;     /* Current battery voltage */
const int8_t PIN_BATT_STATUS = 5;       /* Pulled low if charging is active*/

/* I2C */
const int8_t PIN_I2C_SCL = 9;
const int8_t PIN_I2C_SDA = 10;

/* IMU interrupt pins */
const int8_t PIN_LSM6DS_INT = 11;
const int8_t PIN_MC6470_INT = 12;
const int8_t PIN_BNO55_INT = 13;