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

#ifndef __MC6470_MAG_H__
#define __MC6470_MAG_H__

#include "mc6470.h"

uint32_t MC6470_Mag_Init(struct MC6470_Dev_t *dev);


uint32_t MC6470_Mag_I2C_Write(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);
uint32_t MC6470_Mag_I2C_Read(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);


#endif