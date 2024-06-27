#pragma once

/*! *********************************************************
* @brief Enum to define battery levels
* @enum {BatStatus}
*************************************************************/
enum BatStatus {
    BAT_LOW,    
    BAT_OK,
    BAT_HIGH
};

/*! *********************************************************
* @brief Struct to store current HeadMouse device state.
* @param is_connected TRUE if connected to host over BLE. FALSE otherwise.
* @param is_charging TRUE if currently charging. FALSE otherwise.
* @param is_calibrated TRUE if calibration of gyro has been finished. FALSE otherwise.
* @param bat_status Stores current battery level 
*************************************************************/
typedef struct HmStatus {
    bool is_connected = false;
    bool is_charging = false;
    bool is_calibrated = false;
    BatStatus bat_status = BAT_LOW;
};