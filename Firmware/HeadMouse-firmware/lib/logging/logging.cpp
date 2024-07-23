#include <Arduino.h>
#include "logging.hpp"
#include "hm_board_config_v1_0.hpp"


static constexpr uint32_t MAX_LOG_MSG_LENGTH = 256;



/************************************************************
 * @brief Initialize the serial interface for logging.
 *
 * This function starts the serial interface with a predefined 
 * baud rate and waitsfor the serial port to open before proceeding.
*************************************************************/
void log_init_serial(){
            /* Start serial interface if logging is active */
    Serial.begin(SERIAL_BAUD_RATE);
    while (!Serial) delay(10);  /* Wait for serial port to open */
}

/************************************************************
 * @brief Log a message with a specified log level.
 *
 * This function formats and logs a message to the serial interface 
 * based on the specified log level. The message is formatted using 
 * a printf-style format string and a variable number of arguments.
 *
 * @param level The log level of the message (e.g., LOG_DEBUG, LOG_INFO, 
 *              LOG_WARNING, LOG_ERROR).
 * @param format The printf-style format string for the message.
 * @param ... The variable number of arguments to format the message.
*************************************************************/
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