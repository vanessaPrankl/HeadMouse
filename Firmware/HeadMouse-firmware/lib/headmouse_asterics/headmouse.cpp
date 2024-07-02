#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "headmouse.hpp"
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
HeadMouse::HeadMouse(){}

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
* @return ERR_xxx if something went wrong, OK otherwise.
*************************************************************/
err HeadMouse::init(HmPreferences preferences){
    err error = ERR_GENERIC;

    /* Setup HM preferences */
    error = setPreferences(preferences);
    if(error) return error;

    /* Start serial interface if logging is active */
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
        log_message(LOG_DEBUG, "BNO055 ready!");
    }
    else{
        log_message(LOG_WARNING, "Cannot connect to BNO055");
        return ERR_CONNECTION_FAILED;
    }

    return OK;
}

err HeadMouse::update(){
    err error = ERR_GENERIC;


}
err HeadMouse::pairNewDevice(){

}
err HeadMouse::switchPairedDevice(){

}

/* Setter */

/*! *********************************************************
* @brief Set HeadMouse device preferences
* @param preferences Struct containing device preferences.
* @return ERR_xxx if something went wrong, OK otherwise.
*************************************************************/
err HeadMouse::setPreferences(HmPreferences preferences){
    setMode(preferences.mode);
    setSensitivity(preferences.sensititvity);
    for(int i=0; i<3; i++){
        err error = setButtonAction(preferences.buttons[i].pin, preferences.buttons[i].action);
        if(error) return error;
    }
    return OK;
}

/*! *********************************************************
* @brief Set HeadMouse motion sensitivity
* @param sensitivity Sensititvity level for head motion detection
* @return None
*************************************************************/
void HeadMouse::setSensitivity(devSensitivity sensititvity){
    _preferences.sensititvity = sensititvity;
}

/*! *********************************************************
* @brief Set HeadMouse operation mode
* @param mode Device operation mode
* @return None
*************************************************************/
void HeadMouse::setMode(devMode mode){
    _preferences.mode = mode;
}

/*! *********************************************************
* @brief Set HeadMouse button pins and according device actions.
* @param pinNr uC pin number of button
* @param action Device action associated with button
* @return ERR_OUT_OF_RANGE if pin is no button, OK otherwise.
*************************************************************/
err HeadMouse::setButtonAction(pin pinNr, btnAction action){
    /* Check if pin is actually a button */
    if((pinNr == PIN_BTN_1) || (pinNr == PIN_BTN_1) || (pinNr == PIN_BTN_1) || (pinNr == PIN_BTN_1))
    {
        log_message(LOG_INFO, "Set pin %d to action %d", pinNr, action);
        _preferences.buttons[pinNr-1].pin = pinNr;
        _preferences.buttons[pinNr-1].action = action;
        return OK; //TODO
    }
    else{
        log_message(LOG_ERROR, "Pin %d not a button", pinNr);
        return ERR_OUT_OF_RANGE;
    }
}

/* Getter */

/*! *********************************************************
* @brief Update current device status.
*       (battery, IMU calibration, charging active, BLE connection)
* @return Device status struct
*************************************************************/
HmStatus HeadMouse::getDevStatus(){
    _status.bat_status = getBatStatus();
    _status.is_calibrated = isCalibrated();
    _status.is_charging = isCharging();
    _status.is_connected = isConnected();

    return _status;
}

/*! *********************************************************
* @brief Read current battery voltage and convert it to 
*        according battery status.
* @return Battery status
*************************************************************/
BatStatus HeadMouse::getBatStatus(){
    int32_t adc_value = analogRead(PIN_VBATT_MEASURE);
    float_t voltage = 2 * adc_value * 3.3 / 4095; // "2*" because of 50:50 voltage divider
    if(voltage >= BAT_FULL_V)       _status.bat_status = BAT_FULL;
    else if(voltage >= BAT_HIGH_V)  _status.bat_status = BAT_HIGH;
    else if(voltage >= BAT_OK_V)    _status.bat_status = BAT_OK;
    else if(voltage >= BAT_LOW_V)   _status.bat_status = BAT_LOW;
    else                            _status.bat_status = BAT_CRITICAL;

    return _status.bat_status;
}

/*! *********************************************************
* @brief Check if gyro calibration of IMU has been finished.
* @note If IMU communication fails, there is no handle in this function.
* @return TRUE, if finished, FALSE otherwise.
*************************************************************/
bool HeadMouse::isCalibrated(){
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    log_message(LOG_DEBUG_IMU, "Calibration Sys: %d, GYR: %d, ACC: %d, MAG: %d", system, gyro, accel, mag);

    if(gyro == 3) return true;
    else return false;
}

/*! *********************************************************
* @brief Check if device is connected to host via bluetooth.
* @return TRUE, if connected, FALSE otherwise.
*************************************************************/
bool HeadMouse::isConnected(){
    if(bleMouse.isConnected()){
       return true;
    }
    else return false;
}

/*! *********************************************************
* @brief Check if device battery is currently charging.
* @note Charging means that the power supply is connected and the battery is not full.
* @return TRUE, if charging, FALSE otherwise.
*************************************************************/
bool HeadMouse::isCharging(){
    _status.is_charging = !digitalRead(PIN_BATT_STATUS);
    log_message(LOG_DEBUG_BAT, "Bat is charging: %d", _status.is_charging);

    return _status.is_charging;
}


