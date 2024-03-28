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

#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#include "mc6470_arduino.hpp"


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

#include <Arduino.h>
#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"

#include "mc6470_arduino.hpp"

ArduinoMC6470::ArduinoMC6470(TwoWire *i2c, MC6470_Address_e address)
{
    this->i2c = i2c;
    dev.ctx = this;
    MC6470_Init(&dev, address);
};

ArduinoMC6470::~ArduinoMC6470()
{
};

uint32_t ArduinoMC6470::begin()
{
    return !MC6470_IS_ERROR(MC6470_begin(&dev));
};


void ArduinoMC6470::setStream(Stream *output)
{
    this->output = output;
};

uint32_t ArduinoMC6470::getData(MC6470_MagReading &mag_data, MC6470_AccelReading &accel_data)
{
    uint32_t result = 0;
    result |= MC6470_getData(&dev, &mag_data, &accel_data);
    return result;
};

uint32_t ArduinoMC6470::readIDs()
{
    uint32_t result = 0;
    result |= MC6470_check_ids(&dev);
    return result;
}


void MC6470_delay_ms(unsigned long milliseconds)
{
    delay(milliseconds);
}

void MC6470_delay_us(unsigned long microseconds)
{
    delayMicroseconds(microseconds);
}

// extern "C" int M_DRV_I2C_Write(uint8_t bI2CAddr, uint8_t bRegAddr, uint8_t *pbRegDataBuf, uint8_t bLength)
uint32_t MC6470_I2C_Write(MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    if(dev == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    if(dev->ctx == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    ArduinoMC6470 *mc6470 = (ArduinoMC6470*)dev->ctx;
    if(mc6470->i2c == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }

    mc6470->i2c->beginTransmission(address);
    mc6470->i2c->write(reg_address);
    size_t i = 0;
    for(; i < buffer_length; i++) {
        mc6470->i2c->write(buffer[i]);
    }
    mc6470->i2c->endTransmission();
    if(i == buffer_length)
    {
        return MC6470_Status_OK;
    }
    return MC6470_Status_Count_Mismatch_ERROR;
};

/** Read/poll data from sensor through I2C bus.
 *
 * General poll function for chip data information.
 * The regAdd parameter is the starting register offset where
 * the read should begin, and length is the number of bytes
 * that should be read starting from the regAdd offset.  Data
 * will be returned to the caller via the recvData pointer into
 * a buffer that is at least as large as length.
 
 * @param[in] slaveAdd The 7bit I2C slave address for the sensor to be accessed.
 * @param[in] regAdd The register offset from which to start polling data.
 * @param[in] length The number of bytes to be polled.
 * @param[out] *recvData A pointer to a storage array for receiving the
 *                       bytes read from the bus.
 * @return mCubeResult_t indicating success or failure.
 */
// extern "C" int M_DRV_I2C_Read(uint8_t bI2CAddr, uint8_t bRegAddr, uint8_t *pbDataBuf, uint8_t bLength)
uint32_t MC6470_I2C_Read(MC6470_Dev_t *dev, MC6470_Address_e address, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{

    if(dev == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    if(dev->ctx == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    ArduinoMC6470 *mc6470 = (ArduinoMC6470*)dev->ctx;
    if(mc6470->i2c == nullptr)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    mc6470->i2c->beginTransmission(address);
    mc6470->i2c->write(reg_address);
    mc6470->i2c->endTransmission();

    mc6470->i2c->requestFrom(address, buffer_length);
    size_t i = 0;;
    for(; i < buffer_length; i++)
    {
        buffer[i] = mc6470->i2c->read();
    }
    if(i == buffer_length)
    {
        return MC6470_Status_OK;
    }
    return MC6470_Status_Count_Mismatch_ERROR;
    
};

int MC6470_printf(MC6470_Dev_t *dev, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int space = vsnprintf(NULL, 0, format, args) + 1;
    char buf[space];
    memset(buf, 0, space);
    int len = vsnprintf(buf, space, format, args);
    va_end(args);

    if(dev == nullptr)
    {
        return -MC6470_Status_Null_PTR_ERROR;
    }
    if(dev->ctx == nullptr)
    {
        return -MC6470_Status_Null_PTR_ERROR;
    }    
    ArduinoMC6470 *mc6470 = (ArduinoMC6470*)dev->ctx;
    if(mc6470->output == nullptr)
    {
        return -MC6470_Status_Null_PTR_ERROR;
    }
    return mc6470->output->write((uint8_t*)buf, len);
};