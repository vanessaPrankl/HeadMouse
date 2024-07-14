/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the BLE-mouse in relative cursor 
/*              position mode.
/* Author: Vanessa Prankl
/* Date: 04.04.2024
/* 
/************************************************************************/

#if !defined( ESP32 )
	#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include "program_selector.h"
#ifdef HW_INTERRUPT_TEST 
#include <Arduino.h>
#include <Wire.h>
#include "pin_config_board_v1.h"
#include "BleMouse.h"
#include "ESP32TimerInterrupt.h"

constexpr uint16_t BTN_DEBOUNCE_MS = 25;
constexpr uint16_t BLINK_LED_MS = 500;


// Init ESP32 timer 1
ESP32Timer BtnTimer(0);
ESP32Timer LedBlinkTimer(1);

volatile bool button_click = false;
volatile bool button_press = false;
volatile uint32_t count = 0;

void initButtons();
void initLeds();
bool callbackTimerLed(void *);
bool callbackTimerBtn(void *);
void callbackBtnPress();

/* INIT *****************************************************************/
void setup() {
  Serial.begin(115200);
  delayMicroseconds(1000000);  /* Let serial interface start up */
   
  /* Init peripherals */
  initLeds();
  initButtons();

  /* Init timers */
  if (BtnTimer.attachInterruptInterval(BTN_DEBOUNCE_MS * 1000, callbackTimerBtn));
  BtnTimer.stopTimer();
  if (LedBlinkTimer.attachInterruptInterval(BLINK_LED_MS * 1000, callbackTimerLed));

  /* Init I2C */
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL, 400000);
  delay(10);  /* Let I2C bus come up */

  Serial.print("\nONLINE!");

}


/* MAIN ******************************************************************/
void loop() {
  static uint32_t count_buf = 0;

  if ((count != 0) && (count%5 == 0) && (count != count_buf)){
    count_buf = count;
    Serial.println(count);

    if(button_press){
    Serial.print("\nPressed");
    }
  }
 
  if(button_click){
    Serial.print("\nClicked!");
    button_click = false;
  }
}

void initButtons(){
  /* Button for mouse left click */
  pinMode(PIN_BTN_4, INPUT_PULLUP);

  /* Init pin change interrupt for click detection */
  attachInterrupt(PIN_BTN_4, callbackBtnPress, FALLING);

}

void initLeds(){
  pinMode(PIN_LED_STATUS_R, OUTPUT);
  pinMode(PIN_LED_STATUS_G, OUTPUT);
}

bool IRAM_ATTR callbackTimerLed(void * timerNo){
  static bool status = 0;
  if(status){ /* Green */
    digitalWrite(PIN_LED_STATUS_R, HIGH);  
    digitalWrite(PIN_LED_STATUS_G, LOW); 
    status = !status;
  }
  else{ /* Red */
    digitalWrite(PIN_LED_STATUS_R, LOW);  
    digitalWrite(PIN_LED_STATUS_G, HIGH); 
    status = !status;
  }

  return true;  // restart timer
}

bool IRAM_ATTR callbackTimerBtn(void * timerNo){
  /* Button still pressed? */
  if(!digitalRead(PIN_BTN_4)){
    count++;
    if((count >= 20) && (!button_press)) button_press = true;  /* Detect button press if it has been pressed for at least 500ms */
    if(count >= 10000){ /* Timeout to prevent overflow */
      count = 0;
      button_press = false;
      BtnTimer.stopTimer();
    }
  }/* Button not pressed any more */
  else if(count == 0){
    /* Ignore this  case, invalid click/press duration */
    BtnTimer.stopTimer();
  }
  else if(count < 20){ /* Only detect click if 25-500ms have passed since button release */
      button_click = true;
      count = 0;
      BtnTimer.stopTimer();
  } 
  else{ /* Complete button press */
    button_press = false;
    count = 0;
    BtnTimer.stopTimer();
  }

  return true;
}

void IRAM_ATTR callbackBtnPress(){
  BtnTimer.restartTimer();
}

#endif