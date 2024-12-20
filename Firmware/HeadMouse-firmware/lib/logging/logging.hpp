/* Logtypes: DEBUG, INFO, WARNING, ERROR, and CRITICAL */
#pragma once
#include <Arduino.h>

/* LOG-LEVELS ***************************************************/
//#define LOG_OVER_SERIAL     
#define LOG_LEVEL_DEBUG     0        // General debug information excluding other debug keywords
#define LOG_LEVEL_DEBUG_IMU 0      // IMU debug information only
#define LOG_LEVEL_DEBUG_BAT 0     // Battery debug information only
#define LOG_LEVEL_INFO      0
#define LOG_LEVEL_WARNING   0
#define LOG_LEVEL_ERROR     0

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