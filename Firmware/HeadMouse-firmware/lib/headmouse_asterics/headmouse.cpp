#include <Arduino.h>
#include <Wire.h>
#include <utility/imumaths.h>
#include <Preferences.h>
#include "headmouse.hpp"
#include "BleMouse.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BNO055.h"
#include "logging.hpp"
#include "hw_isr.hpp"

namespace _headmouse{
    Preferences nonVolatileMemory;
    Adafruit_BNO055 bno = Adafruit_BNO055(BNO055_SENSOR_ID, BNO055_I2C_ADDRESS, &Wire);
    BleMouse bleMouse(DEVICE_NAME, DEVICE_MANUFACTURER, BAT_LEVEL_DUMMY);
    volatile bool _measurement_available = 0;
}

namespace isr{
    ESP32Timer ProgramCycleTimer(2);
}
using namespace isr;
using namespace _headmouse;
using namespace preferences;

/************************************************************
 * @brief Timer callback function for program cycle timer.
 *
 * This function is called by the timer interrupt indicate a new
 * program/IMU measurement cycle
 *
 * @param timerNo The timer number (unused).
 * @return Always returns true.
 *************************************************************/
bool IRAM_ATTR HeadMouse::_callbackTimerProgramCycle(void * timerNo){
    _measurement_available = true;
    return true;
}

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
    /* Init buttons */
    _buttons->initPins();
    _buttons->enableButtonInterrupts();

    /* Init leds */
    _leds->init();
    
    /* Init battery charging status input */
    pinMode(PIN_BATT_STATUS, INPUT);

    /* Init I2C connection for IMU */
    pinMode(PIN_I2C_SDA, INPUT); // Disable internal pull-up, external pull-ups set
    pinMode(PIN_I2C_SCL, INPUT); // Disable internal pull-up, external pull-ups set
    if(!Wire.setPins(PIN_I2C_SDA, PIN_I2C_SCL)){
        _status.is_error = true;
        log_message(LOG_ERROR, "Cannot init I2C bus.");
    } 
}

/************************************************************
 * @brief Initialize device preferences at HeadMouse startup.
 * 
 * This function initializes the necessary HeadMouse device 
 * preferences for operating mode, sensitivity and button 
 * to mouse action associations. If already stored on the device
 * the available device config will be loaded. Otherwise standard
 * config will be used and stored in non-volatile storage of the 
 * device. 
 * 
 * @return None
 *************************************************************/
void HeadMouse::_initPreferences(HmPreferences preferences){
    nonVolatileMemory.begin("device_config", false);  // Open preferences namespace in read/write mode
    
    /* Read already stored config or store to permanent device storage if not already done */
    if(nonVolatileMemory.isKey(STORE_MODE)){
        _preferences.mode = (devMode)(nonVolatileMemory.getUInt(STORE_MODE, 0));
        log_message(LOG_INFO, "...MODE Preferences loaded from memory: %d", _preferences.mode);
    } else{ 
        _preferences.mode = preferences.mode;
        nonVolatileMemory.putUInt(STORE_MODE, _preferences.mode);
        log_message(LOG_INFO, "...MODE default preferences set: %d", _preferences.mode);
    }

    if(nonVolatileMemory.isKey(STORE_SENSITIVITY)){
        devSensitivity sensitivity = nonVolatileMemory.getUInt(STORE_SENSITIVITY, 0);
        if((sensitivity >= SENSITIVITY_MIN) && (sensitivity <= SENSITIVITY_MAX)){
            _preferences.sensititvity = sensitivity;
            log_message(LOG_INFO, "... SENSITIVITY Preferences loaded from memory: %d", _preferences.sensititvity);
        }
        else{
            _preferences.sensititvity = preferences.sensititvity;
            nonVolatileMemory.putUInt(STORE_SENSITIVITY, _preferences.sensititvity);
            log_message(LOG_INFO, "... Stored SENSITIVITY out of range (%d), setting default value: %d...", sensitivity, _preferences.sensititvity);
        }
    } else{ 
        _preferences.sensititvity = preferences.sensititvity;
        nonVolatileMemory.putUInt(STORE_SENSITIVITY, _preferences.sensititvity);
        log_message(LOG_INFO, "...SENSITIVITY default preferences set: %d", _preferences.sensititvity);
    }

    for(int i=0; i<BUTTON_COUNT; i++){
        if(nonVolatileMemory.isKey(STORE_BTN[i])){
            _preferences.btn_actions[i] = (btnAction)(nonVolatileMemory.getUInt(STORE_BTN[i], 0));
            log_message(LOG_INFO, "...BTN%d preferences loaded from memory: %d", i, _preferences.btn_actions[i]);
        } else{ 
            _preferences.btn_actions[i] = preferences.btn_actions[i];
            nonVolatileMemory.putUInt(STORE_BTN[i], _preferences.btn_actions[i]);
            log_message(LOG_INFO, "...BTN%d default preferences set: %d", i, _preferences.btn_actions[i]);
        }
            
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
    }   /* Calibration status changed from false to true */
    else if(_status.is_calibrated && !is_calibrated_buf){ 
        is_calibrated_buf = true;
        log_message(LOG_INFO, "IMU calibration finished.");
    }
    /* Check BLE connection after IMU calibration has finished */
    if(_status.is_calibrated && ((_status.is_connected != is_connected_buf) || first_run_is_connected)){

        if(_status.is_connected){
            _leds->set(LED_STATUS, GREEN);
            log_message(LOG_INFO, "Device connected.");
        }
        else if(first_run_is_connected){
            _leds->set(LED_STATUS, BLINK_GREEN);
            log_message(LOG_INFO, "Connecting...");
        }
        /* Connection lost => reconnect */
        else if((_status.is_connected==false) && (is_connected_buf==true) && (!first_run_is_connected)){   
            _leds->set(LED_STATUS, BLINK_GREEN);
            log_message(LOG_INFO, "Device connection lost, reconnecting...");
        }

        first_run_is_connected = false;
        is_connected_buf = _status.is_connected;        
    }      
}

/* PUBLIC METHODS */
/************************************************************
 * @brief Check if new BNO055 measurement cycle has finished
 * 
 * @return TRUE if new data is available, FALSE otherwise.
 *************************************************************/
bool HeadMouse::isMeasurementAvailable(){
    if(_measurement_available){
        return true;
        _measurement_available = false;
    }
    else return false;
}

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
    _preferences.mode = preferences.mode;    // Testing only 
    _preferences.sensititvity = preferences.sensititvity;
    _preferences.btn_actions[0] = preferences.btn_actions[0];
    _preferences.btn_actions[1] = preferences.btn_actions[1];
    _preferences.btn_actions[2] = preferences.btn_actions[2];
    _preferences.btn_actions[3] = preferences.btn_actions[3];
    //_initPreferences(preferences);
    log_message(LOG_INFO, "...Preferences initialized");

    /* Init uC peripherals */
    _initPins();
    log_message(LOG_INFO, "...Pins initialized");

    if(ProgramCycleTimer.attachInterruptInterval(PROGRAM_CYCLE_INTERVAL_MS*1000, _callbackTimerProgramCycle))
    {    
        log_message(LOG_INFO, "...Program cycle timer initialized"); 
    }
    else{
        log_message(LOG_INFO, "... Cannot init Program cycle timer, aborting..."); 
        return ERR_GENERIC;
    }

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
    _status.is_calibrated = isCalibrated();
    _status.is_charging = isCharging();
    _status.is_connected = isConnected();
    updateBatStatus();

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
    static bool first_run = true;
    int64_t mouse_change_x = 0;
    int64_t mouse_change_y = 0;
    int mouse_move_x = 0;
    int mouse_move_y = 0;
    static sensors_event_t imu_data;
    sensors_event_t new_imu_data;
    int32_t sensitivity_level = 0;
    imu::Quaternion quat;
    static imu::Vector<3> euler;
    imu::Vector<3> new_euler;
    
    /* Get a new sensor event */
    quat = bno.getQuat();
    new_euler = quat.toEuler();
    
    //bno.getEvent(&new_imu_data);
    if(first_run){
        first_run = false;
        euler = new_euler;

        /*
        _imu_data.orientation.x = new_imu_data.orientation.x;
        _imu_data.orientation.y = new_imu_data.orientation.y;
        _imu_data.orientation.z = new_imu_data.orientation.z;
        */
    }
    //log_message(LOG_DEBUG_IMU, "new IMU orientation data: X: %.2f, Y: %.2f, Z: %.2f", euler.x(), euler.y(), euler.z());


    /* Determine currently active sensitivity level */
    switch(_preferences.sensititvity){
        case PREF_SENSITIVITY[0]: 
            sensitivity_level = 0;
        break;
            case PREF_SENSITIVITY[1]: 
            sensitivity_level = 1;
        break;
            case PREF_SENSITIVITY[2]: 
            sensitivity_level = 2;
        break;
            case PREF_SENSITIVITY[3]: 
            sensitivity_level = 3;
        break;
            case PREF_SENSITIVITY[4]: 
            sensitivity_level = 4;
        break;
    }

    /* X-AXIS DATA PROCESSING *******************/
    /* Process Euler Angle data */
    if(((new_euler.x() >= 3.14) && (euler.x() < -3.14)) || (new_euler.x() < -3.14) && (euler.x() >= 3.14)){         // Guard edge case
        mouse_change_x = (int64_t)(SCALING_FACTOR*(new_euler.x() + euler.x()));
        log_message(LOG_DEBUG_IMU, "mouse change x: %d", mouse_change_x);
    }
    else if(((new_euler.x() < -0) && (euler.x() >= 0)) || ((new_euler.x() > -0) && (euler.x() <= 0))){    // Guard edge case
        mouse_change_x = (int64_t)(SCALING_FACTOR*(new_euler.x() + euler.x()));
        log_message(LOG_DEBUG_IMU, "mouse change x: %d", mouse_change_x);
    }
    else{
        mouse_change_x = (int64_t)(SCALING_FACTOR*(euler.x() - new_euler.x())); 
        log_message(LOG_DEBUG_IMU, "mouse change x: %d", mouse_change_x);
    }

    /* Add jitter-offset to stabalize mouse when head is not moving */
    if((mouse_change_x >= -JITTER_OFFSET) && (mouse_change_x <= JITTER_OFFSET)){
        mouse_move_x = 0;  
    }
    /* Enter slow-motion mode if mouse is moving slowly to improve positioning accuracy */
    else if((mouse_change_x >= -SLOW_MOTION_OFFSET) && (mouse_change_x <= SLOW_MOTION_OFFSET)){
        for(int i=0; i<SENSITIVITY_STEP_COUNT; i++){
            if((mouse_change_x > SLOWMO_ANGLE_DEFLECTION[i]) && (mouse_change_x <= SLOWMO_ANGLE_DEFLECTION[i+1])){
                mouse_move_x = (int)((mouse_change_x * SLOWMO_SENSITIVITY[i][sensitivity_level]) / 20000);
                break;
            }
            else if((mouse_change_x > -SLOWMO_ANGLE_DEFLECTION[i+1]) && (mouse_change_x <= -SLOWMO_ANGLE_DEFLECTION[i])){
                mouse_move_x = (int)((mouse_change_x * SLOWMO_SENSITIVITY[i][sensitivity_level]) / 20000);
                break;
            }
        }
    }
    /* Normal operation: adjust mouse movement to chosen sensitivity level */
    else{   
        mouse_move_x = (int)((mouse_change_x * _preferences.sensititvity) / 20000);
    }
    log_message(LOG_DEBUG_IMU, "move x: %d", mouse_move_x);
    //log_message(LOG_DEBUG_IMU, "sensitivity: %d", _preferences.sensititvity);

    /* Y-AXIS DATA PROCESSING *******************/
    /* Process Euler Angle data */
    if(((new_euler.z() >= 3.14) && (euler.z() < -3.14)) || (new_euler.z() < -3.14) && (euler.z() >= 3.14)){         // Guard edge case
        mouse_change_y = (int64_t)(SCALING_FACTOR*(new_euler.z() + euler.z()));
        log_message(LOG_DEBUG_IMU, "mouse change y: %d", mouse_change_y);
    }
    else if(((new_euler.z() < -0) && (euler.z() >= 0)) || ((new_euler.z() > -0) && (euler.z() <= 0))){    // Guard edge case
        mouse_change_y = (int64_t)(SCALING_FACTOR*(new_euler.z() + euler.z()));
        log_message(LOG_DEBUG_IMU, "mouse change y: %d", mouse_change_y);
    }
    else{
        mouse_change_y = (int64_t)(SCALING_FACTOR*(new_euler.z() - euler.z())); 
        log_message(LOG_DEBUG_IMU, "mouse change y: %d", mouse_change_y);
    }

    /* Add jitter-offset to stabalize mouse when head is not moving */
    if((mouse_change_y >= -JITTER_OFFSET) && (mouse_change_y <= JITTER_OFFSET)){
        mouse_move_y = 0;  
    }
    /* Enter slow-motion mode if mouse is moving slowly to improve positioning accuracy */
    else if((mouse_change_y >= -SLOW_MOTION_OFFSET) && (mouse_change_y <= SLOW_MOTION_OFFSET)){
        for(int i=0; i<SENSITIVITY_STEP_COUNT; i++){
            if((mouse_change_y > SLOWMO_ANGLE_DEFLECTION[i]) && (mouse_change_y <= SLOWMO_ANGLE_DEFLECTION[i+1])){
                mouse_move_y = (int)((mouse_change_y * SLOWMO_SENSITIVITY[i][sensitivity_level]) / 20000);
                break;
            }
            else if((mouse_change_y > -SLOWMO_ANGLE_DEFLECTION[i+1]) && (mouse_change_y <= -SLOWMO_ANGLE_DEFLECTION[i])){
                mouse_move_y = (int)((mouse_change_y * SLOWMO_SENSITIVITY[i][sensitivity_level]) / 20000);
                break;
            }
        }
    }
    /* Normal operation: adjust mouse movement to chosen sensitivity level */
    else{   
        mouse_move_y = (int)((mouse_change_y * _preferences.sensititvity) / 20000);
    }
    log_message(LOG_DEBUG_IMU, "move y: %d", mouse_move_y);
    //log_message(LOG_DEBUG_IMU, "sensitivity: %d", _preferences.sensititvity);
#ifdef GRAD
    /* X-AXIS DATA PROCESSING *******************/
    /* Process Euler Angle data */
    if((new_imu_data.orientation.x > 359.5) && (imu_data.orientation.x < 0.5)){         // Guard edge case
        mouse_change_x = (int64_t)(SCALING_FACTOR*((new_imu_data.orientation.x - 360.0) - imu_data.orientation.x));
    }
    else if((new_imu_data.orientation.x < 0.5) && (imu_data.orientation.x > 359.5)){    // Guard edge case
        mouse_change_x = (int64_t)(SCALING_FACTOR*(new_imu_data.orientation.x - (imu_data.orientation.x - 360.0)));
    }
    else{
        mouse_change_x = (int64_t)(SCALING_FACTOR*(new_imu_data.orientation.x - imu_data.orientation.x)); 
    }

    /* Add jitter-offset to stabalize mouse when head is not moving */
    if((mouse_change_x >= -JITTER_OFFSET) && (mouse_change_x <= JITTER_OFFSET)){
        mouse_move_x = 0;  
    }
    /* Enter slow-motion mode if mouse is moving slowly to improve positioning accuracy */
    else if((mouse_change_x >= -SLOW_MOTION_OFFSET) && (mouse_change_x <= SLOW_MOTION_OFFSET)){
        for(int i=0; i<SENSITIVITY_STEP_COUNT; i++){
            if((mouse_change_x > SLOWMO_ANGLE_DEFLECTION[i]) && (mouse_change_x <= SLOWMO_ANGLE_DEFLECTION[i+1])){
                mouse_move_x = (int)((mouse_change_x * SLOWMO_SENSITIVITY[i][sensitivity_level]) / SCALING_FACTOR);
                break;
            }
            else if((mouse_change_x > -SLOWMO_ANGLE_DEFLECTION[i+1]) && (mouse_change_x <= -SLOWMO_ANGLE_DEFLECTION[i])){
                mouse_move_x = (int)((mouse_change_x * SLOWMO_SENSITIVITY[i][sensitivity_level]) / SCALING_FACTOR);
                break;
            }
        }
    }
    /* Normal operation: adjust mouse movement to chosen sensitivity level */
    else{   
        mouse_move_x = (int)((mouse_change_x * _preferences.sensititvity) / SCALING_FACTOR);
    }
    /* Y-AXIS DATA PROCESSING *******************/
    /* Process Euler Angle data - IMU z-axis is translated into display y-axis; No special edge case guard needed */
    mouse_change_y = (int64_t)(SCALING_FACTOR*(imu_data.orientation.z - new_imu_data.orientation.z));  

    /* Add jitter-offset to stabalize mouse when head is not moving */
    if((mouse_change_y >= -JITTER_OFFSET) && (mouse_change_y <= JITTER_OFFSET)){
        mouse_move_y = 0;  
    } 
    /* Enter slow-motion mode if mouse is moving slowly to improve positioning accuracy */
    else if((mouse_change_y >= -SLOW_MOTION_OFFSET) && (mouse_change_y <= SLOW_MOTION_OFFSET)){
        for(int i=0; i<SENSITIVITY_STEP_COUNT; i++){
            if((mouse_change_y > SLOWMO_ANGLE_DEFLECTION[i]) && (mouse_change_y <= SLOWMO_ANGLE_DEFLECTION[i+1])){
                mouse_move_y = (int)((mouse_change_y * SLOWMO_SENSITIVITY[i][sensitivity_level]) / SCALING_FACTOR);
                break;
            }
            else if((mouse_change_y > -SLOWMO_ANGLE_DEFLECTION[i+1]) && (mouse_change_y <= -SLOWMO_ANGLE_DEFLECTION[i])){
                mouse_move_y = (int)((mouse_change_y * SLOWMO_SENSITIVITY[i][sensitivity_level]) / SCALING_FACTOR);
                break;
            }
        }
    }
    /* Normal operation: adjust mouse movement to chosen sensitivity level */
    else{       
        mouse_move_y =  (int)((mouse_change_y * _preferences.sensititvity) / SCALING_FACTOR);
    }
    #endif  
    /* Update imu data buffer for later on comparison */
    euler.x() = new_euler.x();
    euler.z() = new_euler.z();  

    /* Move mouse cursor */
    if(_status.is_connected){       
        if((mouse_move_x != 0) || (mouse_move_y != 0)){
            bleMouse.move((unsigned char)(mouse_move_x), (unsigned char)(mouse_move_y),0);  
            log_message(LOG_DEBUG_IMU, "move x: %d", mouse_move_x);
            log_message(LOG_DEBUG_IMU, "move y: %d", mouse_move_y);
        }
    }
    else{
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
        /* Check for left/right mouse button action */
        if((_preferences.btn_actions[i]==RIGHT) || (_preferences.btn_actions[i]==LEFT)){
            if(_buttons->is_click[i]){  /* CLICK */
                bleMouse.click(_preferences.btn_actions[i]);
                _buttons->is_click[i] = false;
                log_message(LOG_INFO, "Button %d clicked ",  i);
            }
            /* Check if button is pressed/released */
            if(_buttons->is_press[i] && !is_press_buf[i]){ /* PRESS */
                bleMouse.press(_preferences.btn_actions[i]);
                is_press_buf[i] = true;
                log_message(LOG_INFO, "Button %d start press ",  i);
            }
            else if(!_buttons->is_press[i] && is_press_buf[i]){ /* RELEASE */
                bleMouse.release(_preferences.btn_actions[i]);
                is_press_buf[i] = false;
                log_message(LOG_INFO, "Button %d stop press ",  i);
            }
        }
        else if(_preferences.btn_actions[i] == SENSITIVITY){
            if(_buttons->is_click[i]){
                if(_preferences.sensititvity == SENSITIVITY_MAX){
                    _preferences.sensititvity = SENSITIVITY_MIN;
                }
                else{ _preferences.sensititvity += SENSITIVITY_STEP;}
                
                setSensitivity(_preferences.sensititvity);
                _buttons->is_click[i] = false;
            }
        }
        else if(_preferences.btn_actions[i] == DEVICE_CONN_AND_CONFIG){
            if(_buttons->is_click[i]){
                bleMouse.connectNewDevice();
                log_message(LOG_INFO, "BLE advertising started...");
                _buttons->is_click[i] = false;
            }
            /* Check if button is pressed/released */
            if(_buttons->is_press[i] && !is_press_buf[i]){ /* PRESS */
                /* TODO: Enter Wifi config mode here*/
                log_message(LOG_INFO, "Button %d startpress  - config mode work in progress ",  i);
                is_press_buf[i] = true;
            }
            else if(!_buttons->is_press[i] && is_press_buf[i]){ /* RELEASE */
                is_press_buf[i] = false;
                log_message(LOG_INFO, "Button %d stop press ",  i);
            }
        }
    }
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
    nonVolatileMemory.putUInt(STORE_SENSITIVITY, _preferences.sensititvity);

    log_message(LOG_INFO, "Sensitivity set to %d", _preferences.sensititvity);
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
    nonVolatileMemory.putUInt(STORE_MODE, _preferences.mode);
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
        nonVolatileMemory.putUInt(STORE_BTN[i], _preferences.btn_actions[i]);
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
void HeadMouse::updateBatStatus(){
    BatStatus bat_status_new = BAT_LOW;

    /* Get battery voltage level */
    int32_t adc_value = analogRead(PIN_VBATT_MEASURE);
    float_t voltage = 2 * adc_value * 3.3 / 4095; // "2*" because of 50:50 voltage divider
    log_message(LOG_DEBUG_BAT, "Battery voltage is: %.2fV", voltage);

    /* Determine new battery level status */
    if(!_status.is_charging){ /* Not charging -> bat level decreasing */
        switch(_status.bat_status){ 
            case BAT_LOW:
                if(voltage >= BAT_HIGH_V)                           bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V + BAT_HYSTERESIS_V)     bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            case BAT_OK:
                if(voltage >= BAT_HIGH_V + BAT_HYSTERESIS_V)        bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V)                        bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            case BAT_HIGH:
                if(voltage >= BAT_HIGH_V)                           bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V)                        bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            default: 
                _status.is_error = true;
        }
    }
    else{  
        switch(_status.bat_status){  /* Charging -> bat level increasing */
            case BAT_LOW:
                if(voltage >= BAT_HIGH_V)                           bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V)                        bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            case BAT_OK:
                if(voltage >= BAT_HIGH_V)                           bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V - BAT_HYSTERESIS_V)     bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            case BAT_HIGH:
                if(voltage >= BAT_HIGH_V - BAT_HYSTERESIS_V)        bat_status_new = BAT_HIGH;
                else if(voltage >= BAT_OK_V)                        bat_status_new = BAT_OK;
                else                                                bat_status_new = BAT_LOW;
            break;
            default: 
                _status.is_error = true;
        }
    }

    if(bat_status_new != _status.bat_status){   /* Only store new battery status if it has changed */
        _status.bat_status = bat_status_new;
    }
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

    //log_message(LOG_DEBUG_IMU, "Calibration Sys: %d, GYR: %d, ACC: %d, MAG: %d", system, gyro, accel, mag);

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


