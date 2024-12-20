/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse in relative cursor 
/*              position mode.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/
#include "program_selector.h"
#ifdef RELATIVE_MOUSE_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "timerEvent.h"
#include "pin_config_board_v1.h"
#include "BleMouse.h"

#define IMU_LSM6DSO
//#define IMU_MC6470

#ifdef IMU_MC6470
#include "mc6470_arduino.hpp"
#endif

#ifdef IMU_LSM6DSO
#include "SparkFunLSM6DSO.h"
#endif



constexpr uint16_t BTN_DEBOUNCE_MS = 25;
constexpr uint16_t BLINK_LED_MS = 500;
constexpr int8_t ACCEL_OFFSET_DEG = 5;

volatile uint8_t button_click = MOUSE_NONE;

#ifdef IMU_MC6470
ArduinoMC6470 mc6470(&Wire, MC6470_ACCEL_ADDRESS_GND);
#endif

#ifdef IMU_LSM6DSO
LSM6DSO lsm6dso;
#endif

BleMouse bleMouse("HeadMouse V1", "FH Technikum Wien", 100);
TimerEvent timerDebounce; 
TimerEvent timerLed;

void initButtons();
void initLeds();
void callback_timerLed();
void callback_left_click();
void callback_timerDebounce();

/* INIT *****************************************************************/
void setup() {
  Serial.begin(115200);
  bleMouse.deviceName = "HeadMouse V1";
  bleMouse.deviceManufacturer = "FH Technikum Wien";
  bleMouse.setBatteryLevel(100);  //TODO Measure battery level here
  bleMouse.begin();

  /* Init peripherals */
  initLeds();
  initButtons();

  timerLed.set(BLINK_LED_MS, callback_timerLed);
  timerDebounce.set(BTN_DEBOUNCE_MS, callback_timerDebounce);
  timerDebounce.disable();

   /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */

  delay(5000); 
  Serial.println("ONLINE!");

  /* Init IMU */
#ifdef IMU_MC6470
  uint32_t err = mc6470.begin();
  if(err != MC6470_Status_OK){
    Serial.println("ERR - MC6470 not reachable");
  }else{
    Serial.println("MC6470 ready");
  }
#endif

#ifdef IMU_LSM6DSO
  if(lsm6dso.begin(0x6A, Wire)){
    Serial.println("LSM6DSO ready");
  }
  else{
    Serial.println("LSM6DSO not reachable");
  }

  if(lsm6dso.initialize(BASIC_SETTINGS)){
    Serial.println("LSM6DSO initialized");
  }
  else{
    Serial.println("Canno init LSM6DSO");
  }
#endif

delay(1000);
}


/* MAIN ******************************************************************/
void loop() {

#ifdef IMU_MC6470
  uint32_t err = MC6470_Status_OK;
  MC6470_MagReading mag_data;
  MC6470_AccelReading acc_data;
#endif


  static int i = 0;

  timerDebounce.update();
  timerLed.update();

  if(bleMouse.isConnected()) {  
    if(button_click){
      Serial.print("\nButton click detected!");
      bleMouse.click(button_click);
      button_click = MOUSE_NONE;
    }
    
    /* Read accel data */
  #ifdef IMU_LSM6DSO

  float acc_data_x = 100*lsm6dso.readFloatAccelX();
  float acc_data_y = 100*lsm6dso.readFloatAccelY();

  /* Process Data */
  if((acc_data_x <= ACCEL_OFFSET_DEG) && (acc_data_x >= -ACCEL_OFFSET_DEG)) acc_data_x = 0;
  if((acc_data_y <= ACCEL_OFFSET_DEG) && (acc_data_y >= -ACCEL_OFFSET_DEG)) acc_data_y = 0;
  /* Move mouse cursor */
  bleMouse.move((unsigned char)(acc_data_x/10), (unsigned char)(acc_data_y/10),0); 

  #endif

  #ifdef IMU_MC6470
    err = mc6470.getData(mag_data, acc_data);
    if(err != MC6470_Status_OK){
      Serial.println("ERR - cannot read data");
    }
  /*
    Serial.print("\nAccelerometer:\n");
    Serial.print(" X = ");
    Serial.println(acc_data.x);
    Serial.print(" Y = ");
    Serial.println(acc_data.y);
    Serial.print(" Z = ");
    Serial.println(acc_data.z);
  */

    /* Process Data */
    if((acc_data.x <= ACCEL_OFFSET_DEG) && (acc_data.x >= -ACCEL_OFFSET_DEG)) acc_data.x = 0;
    if((acc_data.y <= ACCEL_OFFSET_DEG) && (acc_data.y >= -ACCEL_OFFSET_DEG)) acc_data.y = 0;

    /* Move mouse cursor */
    bleMouse.move((unsigned char)(acc_data.y/2), (unsigned char)(acc_data.x/2),0);  
  #endif
  }
        
}

void initButtons(){
  /* Button for mouse left click */
  pinMode(PIN_BTN_1, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_1), callback_left_click, RISING);

}

void initLeds(){
  pinMode(PIN_LED_STATUS_R, OUTPUT);
  pinMode(PIN_LED_STATUS_G, OUTPUT);
}

void callback_timerLed(){
  static bool status = 0;
  if(status){
    digitalWrite(PIN_LED_STATUS_R, HIGH);  
    digitalWrite(PIN_LED_STATUS_G, LOW); 
    status = !status;
  }
  else{
    digitalWrite(PIN_LED_STATUS_R, LOW);  
    digitalWrite(PIN_LED_STATUS_G, HIGH); 
    status = !status;
  }
   
}

void callback_left_click(){
  timerDebounce.reset();
  timerDebounce.enable();

}

void callback_timerDebounce(){
  timerDebounce.disable();
  
  /* Button still pressed? */
  if(digitalRead(PIN_BTN_1)){
    button_click = MOUSE_LEFT;
  }
  else{ /* ignore button press */ }
}

#endif