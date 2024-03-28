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

#ifndef __MC6470_H__
#define __MC6470_H__

#include "mc6470_mag-defs.h"
#include "mc6470_accel-defs.h"

#define MC6470_IS_ERROR(val) ((val & MC6470_Status_ERROR) == MC6470_Status_ERROR )

typedef enum 
{
    MC6470_ACCEL_ADDRESS_GND = 0x4C, /**< Pin A5 set to GND */
    MC6470_ACCEL_ADDRESS_VDD = 0x6C, /**< Pin A5 set to VDD */
    MC6470_MAG_ADDRESS       = 0x0C,
} MC6470_Address_e;


struct MC6470_Dev_t
{
    void *ctx;
    MC6470_Address_e accel_address;
    MC6470_Address_e mag_address;

    MC6470_MAG_CTRL_1_PC_e mag_power_mode;

    MC6470_ACCEL_OUTCFG_RES_e accel_resolution;
    MC6470_ACCEL_OUTCFG_RANGE_e accel_range;
};

typedef struct 
{
    float x;
    float y;
    float z;
} MC6470_MagReading;  

typedef struct 
{
    float x;
    float y;
    float z;
} MC6470_AccelReading;

void MC6470_Init(struct MC6470_Dev_t *dev, MC6470_Address_e address);
uint32_t MC6470_begin(struct MC6470_Dev_t *dev);
uint32_t MC6470_check_ids(struct MC6470_Dev_t *dev);

uint32_t MC6470_getData(struct MC6470_Dev_t *dev, MC6470_MagReading *mag_data, MC6470_AccelReading *accel_data);
uint32_t MC6470_getTemperature(struct MC6470_Dev_t *dev, int8_t *temp);

extern uint32_t MC6470_I2C_Write(struct MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);
extern uint32_t MC6470_I2C_Read(struct MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);
extern int MC6470_printf(struct MC6470_Dev_t *dev, const char *format, ...);
extern void MC6470_delay_ms(unsigned long milliseconds);
extern void MC6470_delay_us(unsigned long microseconds);

#endif