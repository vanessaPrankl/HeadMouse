/* Logtypes: DEBUG, INFO, WARNING, ERROR, and CRITICAL */
#pragma once
#include <Arduino.h>

/* Define following log-levels in preferences file:
#define LOG_LEVEL_DEBUG    1
#define LOG_LEVEL_INFO     1
#define LOG_LEVEL_WARNING  1
#define LOG_LEVEL_ERROR    1
*/

enum LogLevel {
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR,
};

void log_message(LogLevel level, const char *format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    switch (level) {
        #if LOG_LEVEL_DEBUG
        case LOG_DEBUG:
            Serial.print("[DEBUG] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_INFO
        case LOG_INFO:
            Serial.println("[INFO] ");
            Serial.print(buffer);
            break;
        #endif

        #if LOG_LEVEL_WARNING
        case LOG_WARNING:
            Serial.println("[WARNING] ");
            Serial.print(buffer);
            break;
        #endif

        #if LOG_LEVEL_ERROR
        case LOG_ERROR:
            Serial.println("[ERROR] ");
            Serial.print(buffer);
            break;
        #endif

        default:
            break;
    }
}