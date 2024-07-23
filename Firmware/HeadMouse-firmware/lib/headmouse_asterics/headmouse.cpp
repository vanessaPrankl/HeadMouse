#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include "headmouse.hpp"
#include "BleMouse.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "logging.hpp"

namespace _headmouse{

Adafruit_BNO055 bno = Adafruit_BNO055(BNO055_SENSOR_ID, BNO055_I2C_ADDRESS, &Wire);
BleMouse bleMouse(DEVICE_NAME, DEVICE_MANUFACTURER, BAT_LEVEL_DUMMY);
}

using namespace _headmouse;
 
/* PRIVATE METHODS **************************************************/

/************************************************************
 * @brief Initialize microcontroller pins of HeadMouse.
 * 
 * This function initializes the necessary microcontroller pins
 * for LEDs, buttons, battery and the I2C bus for the IMU sensor.
 * 
 * @return None
 *************************************************************/
void HeadMouse::_initPins(){
    /* Init LEDs */
    pinMode(PIN_LED_BAT_R, OUTPUT);
    pinMode(PIN_LED_BAT_G, OUTPUT);
    pinMode(PIN_LED_STATUS_R, OUTPUT);
    pinMode(PIN_LED_STATUS_G, OUTPUT);

    /* Init buttons */
    _buttons->initPins();
    /* Init button interrupts */
    _buttons->enableButtonInterrupts();

    /* Init leds */
    _leds->init();
    
    /* Init battery charging status input */
    pinMode(PIN_BATT_STATUS, INPUT_PULLUP);

    /* Init I2C connection for IMU */
    pinMode(PIN_I2C_SDA, INPUT); // Disable internal pull-up, external pull-ups set
    pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up, external pull-ups set
    if(!Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL)){
        _status.is_error = true;
        log_message(LOG_ERROR, "Cannot init I2C bus.");
    } 
}


/************************************************************
 * @brief Interpret battery state and set corresponding 
 *        battery LED action.
 *
 * This function interprets the current battery state and updates
 * the battery LED status accordingly. It handles different battery
 * levels and charging status.
 * 
 * @return None
 *************************************************************/
void HeadMouse::_batStatusInterpreter(){
    static bool first_run_bat_state = true;
    static bool first_run_is_charging = true;
    static BatStatus bat_state_buf = BAT_LOW;
    static bool is_charging_buf = false;
    
    /* Signal battery charging state */
    if(_status.is_charging){
        if((_status.is_charging != is_charging_buf) || first_run_is_charging){  
            first_run_is_charging = false;
            is_charging_buf = _status.is_charging;
            first_run_bat_state = true; /* Make sure new battery charge level is recognized after charging has finished */
            _leds->set(LED_BATTERY, BLINK_ORANGE);
            log_message(LOG_INFO, "Battery charging active.");
        }
    }
    /* Signal battery charge level */
    else if((_status.bat_status != bat_state_buf) || first_run_bat_state){
        first_run_bat_state = false;
        bat_state_buf = _status.bat_status;

        switch(_status.bat_status){
            case BAT_LOW:
                _leds->set(LED_BATTERY, RED);
                log_message(LOG_INFO, "Battery changed to LOW.");
            break;

            case BAT_OK:
                _leds->set(LED_BATTERY, ORANGE);
                log_message(LOG_INFO, "Battery changed to OK.");
            break;

            case BAT_HIGH:
                _leds->set(LED_BATTERY, GREEN);
                log_message(LOG_INFO, "Battery changed to HIGH.");
            break;

            case BAT_FULL:
                _leds->set(LED_BATTERY, GREEN);
                log_message(LOG_INFO, "Battery changed to FULL.");
            break;

            default:
                _status.is_error = true;
                log_message(LOG_WARNING, "Battery state unknown.");
            break;
        }
    }    
}

/************************************************************
 * @brief Interpret device state and set corresponding status
 *        LED action.
 *
 * This function interprets the current device state, such as error
 * status, calibration status, and connection status, and updates
 * the status LED accordingly.
 *
 * @return None
 *************************************************************/
void HeadMouse::_devStatusInterpreter(){
    static bool first_run_is_calibrated = true;
    static bool first_run_is_connected = true;
    static bool is_connected_buf = false;
    static bool is_calibrated_buf = false;

    
    /* Check if error occured */
    if(_status.is_error){
        
        if(_status.is_error){
            _leds->set(LED_STATUS, BLINK_RED);
            log_message(LOG_ERROR, "Error occured, restart device to reset error.");
        }
        while(1);   /* Device restart needed to reset error */
    }
    
    /* Check if device calibration is active (only needed once during startup) */
    if(!_status.is_calibrated){
        if(first_run_is_calibrated){
            first_run_is_calibrated = false;
            _leds->set(LED_STATUS, BLINK_ORANGE);
            log_message(LOG_INFO, "IMU calibration started...");
        }
        return;
    }
    else if(_status.is_calibrated && !is_calibrated_buf){
        is_calibrated_buf = true;
        log_message(LOG_INFO, "IMU calibration finished.");
    }
    /* Check BLE connection after IMU calibration has finished */
    if(is_calibrated_buf && ((_status.is_connected != is_connected_buf) || first_run_is_connected)){
        first_run_is_connected = false;
        is_connected_buf = _status.is_connected;

        if(_status.is_connected){
            _leds->set(LED_STATUS, GREEN);
            log_message(LOG_INFO, "Device connected.");
        }
        else{
            _leds->set(LED_STATUS, BLINK_GREEN);
            log_message(LOG_INFO, "Device not connected.");
        }
        
    }      
}

/* PUBLIC METHODS */

/************************************************************
 * @brief Initialize HeadMouse hardware components.
 *
 * This function initializes the various hardware components of
 * the HeadMouse, including pins, BLE communication, and the IMU.
 * 
 * @param preferences Struct containing device preferences.
 * @return ERR_xxx if something went wrong, OK otherwise.
 *************************************************************/
err HeadMouse::init(HmPreferences preferences){
    err error = ERR_GENERIC;

    /* Setup HM preferences */
    setPreferences(preferences);
    log_message(LOG_INFO, "...Preferences initialized");

    /* Init uC peripherals */
    _initPins();
    log_message(LOG_INFO, "...Pins initialized");

    /* Start ble task manager for bluetooth mouse communication */
    bleMouse.begin();  
    log_message(LOG_INFO, "...BLE server initialized"); 

    /* Initialise IMU */
    if(bno.begin()){
        log_message(LOG_INFO, "...BNO055 initialized");
    }
    else{
        _status.is_error = true;
        log_message(LOG_ERROR, "...Cannot connect to BNO055");
        return ERR_CONNECTION_FAILED;
    }

    return ERR_NONE;
}


/************************************************************
 * @brief Update current device status.
 *
 * This function updates the current status of the device,
 * including battery status, IMU calibration, charging status,
 * and BLE connection.
 *
 * @return Device status struct.
 *************************************************************/
HmStatus HeadMouse::updateDevStatus(){
    _status.bat_status = getBatStatus();
    _status.is_calibrated = isCalibrated();
    _status.is_charging = isCharging();
    _status.is_connected = isConnected();

    log_message(LOG_DEBUG, "Status: \nisCharging: %d\nBatStatus: %d, \nisCalibrated: %d, \nisConnected: %d", 
    _status.is_charging, _status.bat_status, _status.is_calibrated, _status.is_connected);

    _batStatusInterpreter();
    _devStatusInterpreter();

    return _status;
}

/************************************************************
 * @brief Update IMU data and translate it into mouse movements.
 *
 * This function updates the IMU data and translates the head
 * movements into mouse cursor movements.
 *
 * @return ERR_xxx if something went wrong, OK otherwise.
 *************************************************************/
err HeadMouse::updateMovements(){
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

    log_message(LOG_DEBUG_IMU, "IMU orientation data: X: %.2f, Y: %.2f, Z: %.2f\n", new_imu_data.orientation.x, new_imu_data.orientation.y, new_imu_data.orientation.z);

    //displayCalStatus();

    /* Process data */
    move_mouse_x = (int)(_preferences.sensititvity*new_imu_data.orientation.x) - (int)(_preferences.sensititvity*_imu_data.orientation.x);
    move_mouse_y = (int)(_preferences.sensititvity*_imu_data.orientation.y) - (int)(_preferences.sensititvity*new_imu_data.orientation.y);

    /* Store orientation values into buffer for later on comparison */
    _imu_data.orientation.x = new_imu_data.orientation.x;
    _imu_data.orientation.y = new_imu_data.orientation.y;

    /* Move mouse cursor */
    if(_status.is_connected){
        bleMouse.move((unsigned char)(move_mouse_x), (unsigned char)(move_mouse_y),0);  
        return ERR_CONNECTION_FAILED;
    }

    return ERR_NONE;
}

/************************************************************
 * @brief Update button actions.
 *
 * This function checks the button states and performs the
 * corresponding actions based on clicks or presses.
 *************************************************************/
/* TODO */
void HeadMouse::updateBtnActions(){
    static bool is_press_buf[BUTTON_COUNT] = {0};
    
    for(int i=0; i<BUTTON_COUNT; i++){
        /* Only execute click/press if button is associated with left or right mouse button  */
        if((_preferences.btn_actions[i]==RIGHT) || (_preferences.btn_actions[i]==LEFT)){
            if(_buttons->is_click[i]){  /* CLICK */
                bleMouse.click(_preferences.btn_actions[i]);
                _buttons->is_click[i] = false;
                log_message(LOG_DEBUG, "Button %d clicked ",  i);
            }
            /* Only execute press if button is associated with left or right mouse button  */
            else if(_buttons->is_press[i] && !is_press_buf[i]){ /* PRESS */
                bleMouse.press(_preferences.btn_actions[i]);
                is_press_buf[i] = true;
                log_message(LOG_DEBUG, "Button %d start press ",  i);
            }
            else if(!_buttons->is_press[i] && is_press_buf[i]){ /* RELEASE */
                bleMouse.release(_preferences.btn_actions[i]);
                is_press_buf[i] = false;
                log_message(LOG_DEBUG, "Button %d stop press ",  i);
            }
        }
    }
}

/************************************************************
 * @brief Pair new device.
 *
 * This function handles the pairing of a new device via BLE.
 * 
 * @note NOT IMPLEMENTED YET!
 *
 * @return ERR_xxx if something went wrong, OK otherwise.
 *************************************************************/
err HeadMouse::pairNewDevice(){
    /* TODO */
    return ERR_GENERIC;
}

/************************************************************
 * @brief Switch paired device.
 *
 * This function handles switching between previously paired
 * devices via BLE.
 * 
 * @note NOT IMPLEMENTED YET!
 *
 * @return ERR_xxx if something went wrong, OK otherwise.
 *************************************************************/
err HeadMouse::switchPairedDevice(){
    /* TODO */
    return ERR_GENERIC;
}

/* SETTER */

/************************************************************
 * @brief Set HeadMouse device preferences.
 *
 * This function sets the various preferences for the HeadMouse
 * device, including mode, sensitivity, and button actions.
 *
 * @param preferences Struct containing device preferences.
 *************************************************************/
void HeadMouse::setPreferences(HmPreferences preferences){
    setMode(preferences.mode);
    setSensitivity(preferences.sensititvity);
    setButtonActions(preferences.btn_actions);
}

/************************************************************
 * @brief Set HeadMouse motion sensitivity.
 *
 * This function sets the sensitivity level for head motion
 * detection.
 *
 * @param sensitivity Sensititvity level for head motion detection.
 *************************************************************/
void HeadMouse::setSensitivity(devSensitivity sensititvity){
    _preferences.sensititvity = sensititvity;
    log_message(LOG_INFO, "...Sensitivity set to %d", _preferences.sensititvity);
}

/************************************************************
 * @brief Set HeadMouse operation mode.
 *
 * This function sets the operation mode of the HeadMouse device.
 *
 * @param mode Device operation mode.
 *************************************************************/
void HeadMouse::setMode(devMode mode){
    _preferences.mode = mode;
    log_message(LOG_INFO, "...Mode set to %d", _preferences.mode);
}

/************************************************************
 * @brief Set HeadMouse button pins and corresponding actions.
 *
 * This function sets the microcontroller pins for the buttons and
 * assigns the corresponding actions for each button.
 *
 * @param actions Array of actions associated with each button.
 *************************************************************/
void HeadMouse::setButtonActions(btnAction* actions){
    for(int i=0; i<BUTTON_COUNT; i++){
        _preferences.btn_actions[i] = actions[i];
        log_message(LOG_INFO, "...Set pin %d to action %d", i, _preferences.btn_actions[i]);
    }
}


/* GETTER */

/************************************************************
 * @brief Read current battery voltage and convert to battery
 *        status.
 *
 * This function reads the current battery voltage and converts it
 * to a corresponding battery status.
 *
 * @return Battery status.
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

/************************************************************
 * @brief Check if IMU calibration is complete.
 *
 * This function checks if the gyro calibration of the IMU has
 * been completed.
 *
 * @return TRUE if calibration is complete, FALSE otherwise.
 *************************************************************/
bool HeadMouse::isCalibrated(){
    /* Get the four calibration values (0..3) */
    /* Any sensor data reporting 0 should be ignored, */
    /* 3 means 'fully calibrated" */
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);

    log_message(LOG_DEBUG_IMU, "Calibration Sys: %d, GYR: %d, ACC: %d, MAG: %d", system, gyro, accel, mag);

    if((gyro == 3)) return true;
    else return false;
}

/************************************************************
 * @brief Check if device is connected to host via Bluetooth.
 *
 * This function checks if the device is currently connected to a
 * host via Bluetooth.
 *
 * @return TRUE if connected, FALSE otherwise.
 *************************************************************/
bool HeadMouse::isConnected(){
    if(bleMouse.isConnected()){
       return true;
    }
    else return false;
}

/************************************************************
 * @brief Check if device battery is currently charging.
 *
 * This function checks if the device battery is currently being
 * charged.
 *
 * @note Charging means that the power supply is connected and the
 *       battery is not full.
 * @return TRUE if charging, FALSE otherwise.
 *************************************************************/
bool HeadMouse::isCharging(){
    _status.is_charging = !digitalRead(PIN_BATT_STATUS);

    return _status.is_charging;
}


