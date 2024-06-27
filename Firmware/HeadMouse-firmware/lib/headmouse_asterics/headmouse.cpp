#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "./api/headmouse.hpp"
#include "hm_board_config_v1_0.hpp"
#include "default_preferences.hpp"
#include "BleMouse.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "logging.cpp"

namespace _headmouse{

Adafruit_BNO055 bno = Adafruit_BNO055(BNO055_SENSOR_ID, BNO055_I2C_ADDRESS, &Wire);
BleMouse bleMouse(DEVICE_NAME, DEVICE_MANUFACTURER, BAT_LEVEL_DUMMY);
}

using namespace _headmouse;

/*! *********************************************************
* @brief Default HeadMouse constructor
*************************************************************/
HeadMouse::HeadMouse(){
    _preferences.mode = 
}

/*! *********************************************************
* @brief Update IMU data and translate it into mouse movements
*************************************************************/
err HeadMouse::_updateMovements(){
    static int first_run = true;
    int move_mouse_y = 0;
    int move_mouse_x = 0;
    sensors_event_t _imu_data;

    sensors_event_t new_imu_data;

    /* Get a new sensor event */
    bno.getEvent(&new_imu_data);
    if(first_run){
        first_run = false;
        _imu_data.orientation.x = new_imu_data.orientation.x;
        _imu_data.orientation.y = new_imu_data.orientation.y;
        _imu_data.orientation.z = new_imu_data.orientation.z;
    }

    log_message(LOG_DEBUG, "IMU orientation data: X: %.2f, Y: %.2f, Z: %.2f\n", new_imu_data.orientation.x, new_imu_data.orientation.y, new_imu_data.orientation.z);

    //displayCalStatus();

    /* Process data */
    move_mouse_x = (int)(50*new_imu_data.orientation.x) - (int)(50*_imu_data.orientation.x);
    move_mouse_y = (int)(50*_imu_data.orientation.y) - (int)(50*new_imu_data.orientation.y);

    /* Move mouse cursor */
    if(bleMouse.isConnected()){
        bleMouse.move((unsigned char)(move_mouse_x), (unsigned char)(move_mouse_y),0);  
    }

    /* Store orientation values into buffer for later on comparison */
    _imu_data.orientation.x = new_imu_data.orientation.x;
    _imu_data.orientation.y = new_imu_data.orientation.y;

}
void HeadMouse::_updateBtnActions(){
    /* TODO Create interrupt based button action detection */

    bool btn_1 = digitalRead(PIN_BTN_1);
    bool btn_2 = digitalRead(PIN_BTN_2);
    bool btn_3 = digitalRead(PIN_BTN_3);
    bool btn_4 = digitalRead(PIN_BTN_4);

    log_message(LOG_DEBUG, "Button values [0=pressed/1=open]: %d, %d, %d, %d", btn_1, btn_2, btn_3, btn_4);
}

/*! *********************************************************
* @brief Initialize microcontroller pins of HeadMouse
* @return None
*************************************************************/
void HeadMouse::_initPins(){
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

    /* Init I2C connection for IMU */
    pinMode(PIN_I2C_SDA, INPUT); // Disable internal pull-up, external pull-ups set
    pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up, external pull-ups set
    Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL); 
}

/*! *********************************************************
* @brief Initialize HeadMouse hardware components 
* @return ERR_xxx if something went wrong, ERR_NONE otherwise.
*************************************************************/
err HeadMouse::init(){
    err error = ERR_GENERIC;

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || defined(LOG_LEVEL_WARNING) || defined(LOG_LEVEL_ERROR)
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) delay(10);  /* Wait for serial port to open */
    Serial.println("HeadMouse V1"); 
#endif

    /* Init uC peripherals */
    _initPins();

    /* Start ble task manager for bluetooth mouse communication */
    bleMouse.begin();   

    /* Initialise IMU */
    if(bno.begin()){
        error = ERR_NONE;
        log_message(LOG_DEBUG, "BNO055 ready!");
    }
    else{
        error = ERR_CONNECTION_FAILED;
        log_message(LOG_WARNING, "Cannot connect to BNO055");
    }

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO)
    /* Display some basic information on this sensor */
    displaySensorDetails();

    /* Optional: Display current status */
    displaySensorStatus();
#endif
    return error;
}

err HeadMouse::update(){
    err error = ERR_GENERIC;


}
err HeadMouse::pairNewDevice(){

}
err HeadMouse::switchPairedDevice(){

}

/* Getter */
HmStatus HeadMouse::get_status(err*){

}
batStatus HeadMouse::getBatStatus(err*){

}
bool HeadMouse::isCalibrated(err*){

}
bool HeadMouse::isConnected(err*){

}
bool HeadMouse::isCharging(){

}

/* Setter */
err HeadMouse::setSensitivity(devSensitivity){

}
err HeadMouse::setMode(devMode){

}
err HeadMouse::setButtonAction(uint8_t, btnAction){

}
