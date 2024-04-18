/*
 * A C library for the mCube MC6470 6-axis accelerometer and magnetometer sensor.
 * Copyright (C) 2022  eResearch, James Cook University
 * Author: NigelB
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef __MC6470_DEFS_H__
#define __MC6470_DEFS_H__

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

typedef uint8_t MC6470_reg_value;
typedef uint8_t MC6470_reg_addr;

typedef enum 
{
    MC6470_Status_UNKNOWN                = 0,
    MC6470_Status_OK                     = 0b00000001,
    MC6470_Status_ERROR                  = 0b00000010,
    MC6470_Status_Null_Mag_NotFound      = 0b00001010,
    MC6470_Status_Null_Accel_NotFound    = 0b00010010,
    MC6470_Status_Null_PTR_ERROR         = 0b00100010,
    MC6470_Status_Count_Mismatch_ERROR   = 0b01000010,
} MC6470_Status_e;

#define RETURN_ERROR_IF_NULL(ptr) {if(ptr == NULL){return MC6470_Status_Null_PTR_ERROR;}};

#endif