#pragma once

#include <stdint.h>

typedef uint8_t pin;

enum err {
    ERR_NONE,
    ERR_CONNECTION_FAILED,
    ERR_OUT_OF_RANGE,
    ERR_GENERIC    
};