#include <Arduino.h>
#include "logging.hpp"
#include "hm_board_config_v1_0.hpp"


static constexpr uint32_t MAX_LOG_MSG_LENGTH = 256;

void log_init_serial(){
            /* Start serial interface if logging is active */
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) delay(10);  /* Wait for serial port to open */
}

void log_message(LogLevel level, const char *format, ...) {
    static char buffer[MAX_LOG_MSG_LENGTH] = {0};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, MAX_LOG_MSG_LENGTH, format, args);
    va_end(args);

    buffer[MAX_LOG_MSG_LENGTH - 1] = '\0';

    switch (level) {
        #if LOG_LEVEL_DEBUG
        case LOG_DEBUG:
            Serial.print("\n[DEBUG] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_DEBUG_BAT
        case LOG_DEBUG_BAT:
            Serial.print("\n[DEBUG_BAT] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_DEBUG_IMU
        case LOG_DEBUG_IMU:
            Serial.print("\n[DEBUG_IMU] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_INFO
        case LOG_INFO:
            Serial.print("\n[INFO] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_WARNING
        case LOG_WARNING:
            Serial.print("\n[WARNING] ");
            Serial.println(buffer);
            break;
        #endif

        #if LOG_LEVEL_ERROR
        case LOG_ERROR:
            Serial.print("\n[ERROR] ");
            Serial.println(buffer);
            break;
        #endif

        default:
            break;
    }
}