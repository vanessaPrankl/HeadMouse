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

namespace headmouse{

Adafruit_BNO055 bno = Adafruit_BNO055(BNO055_SENSOR_ID, BNO055_I2C_ADDRESS, &Wire);
BleMouse bleMouse(DEVICE_NAME, DEVICE_MANUFACTURER, BAT_LEVEL_DUMMY);
}

using namespace headmouse;

err HeadMouse::updateMovements(){

}
err HeadMouse::updateBtnActions(){

}


err HeadMouse::init(){
#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO) || defined(LOG_LEVEL_WARNING) || defined(LOG_LEVEL_ERROR)
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) delay(10);  /* Wait for serial port to open */
    Serial.println("HeadMouse V1"); 
#endif

    /* Start ble task manager for mouse handle */
    bleMouse.begin();   

    /* Init I2C connection for IMU */
    pinMode(PIN_I2C_SDA, INPUT); // Disable internal pull-up, external pull-ups set
    pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up, external pull-ups set
    Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL);

    /* Initialise IMU */
    if(bno.begin()) log_message(LOG_DEBUG, "BNO055 ready!");
    else            log_message(LOG_WARNING, "Cannot connect to BNO055");

#if defined(LOG_LEVEL_DEBUG) || defined(LOG_LEVEL_INFO)
    /* Display some basic information on this sensor */
    displaySensorDetails();

    /* Optional: Display current status */
    displaySensorStatus();
#endif
}

err HeadMouse::update(){

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
