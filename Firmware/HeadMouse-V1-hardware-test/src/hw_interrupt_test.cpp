/* BLE MOUSE TEST *******************************************************/
/* 
/* Description: This is a test firmware for the hardware timer and pin 
/*              change interrupts of ESP32-S3. The program detects button
/*              click or press at two pins and prints the according action
/*              on terminal. 
/* Author: Vanessa Prankl
/* Date: 17.07.2024
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
constexpr uint32_t BTN_COUNT = 4;


// Init ESP32 timer 1
ESP32Timer BtnTimer(0);
ESP32Timer LedBlinkTimer(1);

struct button {
  const uint8_t pin;
  bool active;
  bool click;
  bool press;

  button(uint8_t p, bool a = false, bool c = false, bool pr = false) 
        : pin(p), active(a), click(c), press(pr) {}
};

volatile button buttons[BTN_COUNT] = {{PIN_BTN_1, false, false, false}, 
                                    {PIN_BTN_2, false, false, false},
                                    {PIN_BTN_3, false, false, false},
                                    {PIN_BTN_4, false, false, false}};
volatile uint32_t count = 0;

void initButtons();
void initLeds();
bool callbackTimerLed(void *);
bool callbackTimerBtn(void *);
void callbackBtnPress();
void enableButtonInterrupts();
void disableButtonInterrupts();

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

  /* Init button interrupts */
  enableButtonInterrupts();

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

    for(int i = 0; i<BTN_COUNT; i++){
      if (buttons[i].press){
        Serial.print("\nPressed: ");
        Serial.print(buttons[i].pin);
      }
    }
  }
 
  for(int i = 0; i<BTN_COUNT; i++){
    if (buttons[i].click){
      Serial.print("\nClicked: ");
      Serial.print(buttons[i].pin);
      buttons[i].click = false;
    }
  }
}

void initButtons(){
  /* Button for mouse left click */
  pinMode(PIN_BTN_1, INPUT_PULLUP);
  pinMode(PIN_BTN_2, INPUT_PULLUP);
  pinMode(PIN_BTN_3, INPUT_PULLUP);
  pinMode(PIN_BTN_4, INPUT_PULLUP);
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
  int index = 0;

  /* Detect which button is active */
  for(int i = 0; i<BTN_COUNT; i++){
    if (buttons[i].active){
      index = i;
      break;
    }
  }

  /* Button still pressed? */
  if(!digitalRead(buttons[index].pin)){
    count++;
    if((count >= 20) && (!buttons[index].press)) buttons[index].press = true;  /* Detect button press if it has been pressed for at least 500ms */
    if(count >= 10000){ /* Timeout to prevent overflow */
      count = 0;
      buttons[index].press = false;
      BtnTimer.stopTimer();
      enableButtonInterrupts();
    }
  }/* Button not pressed any more */
  else if(count == 0){
    /* Ignore this  case, invalid click/press duration */
    BtnTimer.stopTimer();
    enableButtonInterrupts();
  }
  else if(count < 20){ /* Only detect click if 25-500ms have passed since button release */
      buttons[index].click = true;
      count = 0;
      BtnTimer.stopTimer();
      enableButtonInterrupts();
  } 
  else{ /* Complete button press */
    buttons[index].press = false;
    count = 0;
    BtnTimer.stopTimer();
    enableButtonInterrupts();
  }

  return true;
}

void IRAM_ATTR callbackBtn1Press(){
  disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
  buttons[0].active = true;
  BtnTimer.restartTimer();
}

void IRAM_ATTR callbackBtn2Press(){
  disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
  buttons[1].active = true;
  BtnTimer.restartTimer();
}

void IRAM_ATTR callbackBtn3Press(){
  disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
  buttons[2].active = true;
  BtnTimer.restartTimer();
}

void IRAM_ATTR callbackBtn4Press(){
  disableButtonInterrupts();  /* Prevent multiple button actions to be detected */
  buttons[3].active = true;
  BtnTimer.restartTimer();
}

void enableButtonInterrupts() {
  /* Reset button activity first */
  for(int i=0; i<BTN_COUNT; i++){
    buttons[i].active = false;
  }
  
  /* Then start pin change interrupts again */
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_1), callbackBtn1Press, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_2), callbackBtn2Press, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_3), callbackBtn3Press, FALLING);
  attachInterrupt(digitalPinToInterrupt(PIN_BTN_4), callbackBtn4Press, FALLING);
}

// Function to disable interrupts for the button pins
void disableButtonInterrupts() {
  detachInterrupt(digitalPinToInterrupt(PIN_BTN_1));
  detachInterrupt(digitalPinToInterrupt(PIN_BTN_2));
  detachInterrupt(digitalPinToInterrupt(PIN_BTN_3));
  detachInterrupt(digitalPinToInterrupt(PIN_BTN_4));
}

#endif