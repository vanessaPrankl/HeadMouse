/* Logtypes: DEBUG, INFO, WARNING, ERROR, and CRITICAL */
#pragma once
#include <Arduino.h>

/* LOG-LEVELS ***************************************************/
#define LOG_LEVEL_DEBUG    1        // General debug information excluding other debug keywords
#define LOG_LEVEL_DEBUG_IMU 0       // IMU debug information only
#define LOG_LEVEL_DEBUG_BAT 0       // Battery debug informatino only
#define LOG_LEVEL_INFO     1
#define LOG_LEVEL_WARNING  1
#define LOG_LEVEL_ERROR    1

/*! *********************************************************
* @brief Set HeadMouse button pins and according device actions.
* @enum {number}
*************************************************************/
enum LogLevel {
    LOG_DEBUG,
    LOG_DEBUG_BAT,
    LOG_DEBUG_IMU,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

/*! *********************************************************
* @brief Init serial interface for logging
*************************************************************/
void log_init_serial();

/*! *********************************************************
* @brief Print log message and arguments to serial interface.
* @param level According log level
* @param format Log message and format for further arguments
* @param ... further arguments (e.g. numbers)
*************************************************************/
void log_message(LogLevel level, const char *format, ...);