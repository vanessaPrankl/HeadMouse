#pragma once

/************************************************************
* Battery level voltage defintions
*************************************************************/
constexpr float BAT_FULL_V = 4.2;
constexpr float BAT_HIGH_V = 3.9;
constexpr float BAT_OK_V = 3.5;
constexpr float BAT_LOW_V = 3.3;

/*! *********************************************************
* @brief Enum to define battery levels
*************************************************************/
enum BatStatus {
    BAT_FULL,
    BAT_HIGH,
    BAT_OK,
    BAT_LOW, 
    BAT_CRITICAL
};

/*! *********************************************************
* @brief Struct to store current HeadMouse device state.
* @param is_error   TRUE if error occured (eg. hardware component not 
*                   reachable etc.), false if everything working.
* @param is_connected TRUE if connected to host over BLE. FALSE otherwise.
* @param is_charging TRUE if currently charging. FALSE otherwise.
* @param is_calibrated TRUE if calibration of gyro has been finished. 
*                   FALSE otherwise.
* @param bat_status Stores current battery level 
*************************************************************/
typedef struct HmStatus {
    bool is_error = false;
    bool is_connected = false;
    bool is_charging = false;
    bool is_calibrated = false;
    BatStatus bat_status = BAT_LOW;
};