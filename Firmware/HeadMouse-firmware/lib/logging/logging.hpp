/* Logtypes: DEBUG, INFO, WARNING, ERROR, and CRITICAL */
#pragma once
#include <Arduino.h>

/* LOG-LEVELS ***************************************************/
//#define LOG_OVER_SERIAL 
//#define LOG_LEVEL_DEBUG            // General debug information excluding other debug keywords
//#define LOG_LEVEL_DEBUG_IMU       // IMU debug information only
//#define LOG_LEVEL_DEBUG_BAT       // Battery debug informatino only
//#define LOG_LEVEL_INFO     
//#define LOG_LEVEL_WARNING  
//#define LOG_LEVEL_ERROR    

/*! *********************************************************
* @brief Set HeadMouse button pins and according device actions.
*************************************************************/
enum LogLevel {
    LOG_DEBUG,
    LOG_DEBUG_BAT,
    LOG_DEBUG_IMU,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

void log_init_serial();

void log_message(LogLevel level, const char *format, ...);