
/*
 * An Arduino library for the mCube MC6470 6-axis eCompass. 
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

#pragma once

#if defined(__cplusplus)
extern "C"
{
#endif

#include "mc6470.h"

#if defined(__cplusplus)
}
#endif

#include "Wire.h"

class ArduinoMC6470
{
    friend uint32_t MC6470_I2C_Write(MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);
    friend uint32_t MC6470_I2C_Read(MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length);
    friend int MC6470_printf(MC6470_Dev_t *dev, const char *format, ...);
    private:
        MC6470_Dev_t dev;
        Stream *output = nullptr;
        TwoWire *i2c = nullptr;
    public:
        ArduinoMC6470(TwoWire *i2c, MC6470_Address_e address);
        ~ArduinoMC6470();
        uint32_t begin();
        void setStream(Stream *output);
        uint32_t getData(MC6470_MagReading &mag_data, MC6470_AccelReading &accel_data);
        uint32_t readIDs();


};
