#pragma once

enum batStatus {
    BAT_LOW,
    BAT_OK,
    BAT_HIGH
};

typedef struct HmStatus {
    bool is_connected = false;
    bool is_charging = false;
    bool is_calibrated = false;
    batStatus bat_status = BAT_LOW;
};