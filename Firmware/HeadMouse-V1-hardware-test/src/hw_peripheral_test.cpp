/* HARDWARE TEST HEADMOUSE V1 *******************************************/
/* 
/* Description: This is a hardware test firmware for HeadMouse board version 1.
/* Author: Vanessa Prankl
/* Date: 11.01.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef HW_PERIPHERAL_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"

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

void button_test(){
  bool button_1 = digitalRead(PIN_BTN_1);
  bool button_2 = digitalRead(PIN_BTN_2);
  bool button_3 = digitalRead(PIN_BTN_3);
  bool button_4 = digitalRead(PIN_BTN_4);

  Serial.print("Button values [0=pressed/1=open]: ");
  Serial.print(button_1);
  Serial.print(", ");
  Serial.print(button_2);
  Serial.print(", ");
  Serial.print(button_3);
  Serial.print(", ");
  Serial.println(button_4);
}

void battery_measurement_test(){
  bool battery_charging = digitalRead(PIN_BATT_STATUS);
  int32_t adc_value = analogRead(PIN_VBATT_MEASURE);
  float_t battery_voltage = 2 * adc_value * 3.3 / 4095; // "2*" because of 50:50 voltage divider

  Serial.print("Battery charging status [0=ON/1=OFF]: ");
  Serial.println(!battery_charging);
  Serial.print("Battery voltage [V]: ");
  Serial.println(battery_voltage);
}

void led_test(){
  digitalWrite(PIN_LED_BAT_R, HIGH);  
  digitalWrite(PIN_LED_BAT_G, LOW); 
  digitalWrite(PIN_LED_STATUS_R, HIGH);  
  digitalWrite(PIN_LED_STATUS_G, LOW);  

  delay(500);         

  digitalWrite(PIN_LED_BAT_R, LOW);  
  digitalWrite(PIN_LED_BAT_G, HIGH);  
  digitalWrite(PIN_LED_STATUS_R, LOW);  
  digitalWrite(PIN_LED_STATUS_G, HIGH);

  delay(500);         

  digitalWrite(PIN_LED_BAT_R, LOW);  
  digitalWrite(PIN_LED_BAT_G, LOW);  
  digitalWrite(PIN_LED_STATUS_R, LOW);  
  digitalWrite(PIN_LED_STATUS_G, LOW);
}

/* INIT *****************************************************************/
void setup() {

  Serial.begin(9600);

  /* Init LEDs */
  pinMode(PIN_LED_BAT_R, OUTPUT);
  pinMode(PIN_LED_BAT_G, OUTPUT);
  pinMode(PIN_LED_STATUS_R, OUTPUT);
  pinMode(PIN_LED_STATUS_G, OUTPUT);

  /* Init Buttons */
  pinMode(PIN_BTN_1, INPUT_PULLUP);
  pinMode(PIN_BTN_2, INPUT_PULLUP);
  pinMode(PIN_BTN_3, INPUT_PULLUP);
  pinMode(PIN_BTN_4, INPUT_PULLUP);  
  
  /* Init battery charging status input */
  pinMode(PIN_BATT_STATUS, INPUT_PULLUP);

  /* Init I2C (IMUs) */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);

  delay(5000); 
}

/* MAIN ******************************************************************/
void loop() {

  i2c_bus_scan();

  button_test();

  battery_measurement_test();

  led_test();

  delay(1000);       
}
#endif

