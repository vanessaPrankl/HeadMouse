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

#include "mc6470.h"

uint32_t MC6470_Mag_Init(struct MC6470_Dev_t *dev)
{
    return MC6470_Status_OK;
}

uint32_t MC6470_Mag_I2C_Write(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    if(dev == NULL)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }    
    return MC6470_I2C_Write(dev, dev->mag_address, reg_address, buffer, buffer_length);
};

uint32_t MC6470_Mag_I2C_Read(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    if(dev == NULL)
    {
        return MC6470_Status_Null_PTR_ERROR;
    }
    return MC6470_I2C_Read(dev, dev->mag_address, reg_address, buffer, buffer_length);
};


uint32_t MC6470_Mag_hasData(struct MC6470_Dev_t *dev, bool *has_data)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_SR_ADDR;
    uint32_t result = 0;
    uint8_t current = 0;
    
    result |= MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    int v = MC6470_ACCEL_SR_ACQ_INT_GET(current);
    // MC6470_printf(dev, "[MC6470 Accel] Reg Read [0x%02X]: 0x%02X Value: %i\r\n", reg_addr, current, v);
    if(!MC6470_IS_ERROR(result))
    {
        *has_data = v;
    }else{
        *has_data = false;
    }
    return result;
};

uint32_t MC6470_Mag_getData(struct MC6470_Dev_t *dev, float *x, float *y, float *z)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_XOUT_EX_L_ADDR;
    uint8_t data[6] = {0};
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, data, sizeof(data));
    bool has_data = false;
    short _x = 0;
    short _y = 0;
    short _z = 0;
    while(!has_data)
    {
        MC6470_Accel_hasData(dev, &has_data);
    }
    if(!MC6470_IS_ERROR(result))
    {
        _x = data[0] | (data[1] << 8);
        _y = data[2] | (data[3] << 8);
        _z = data[4] | (data[5] << 8);

        *x = (float)_x;
        *y = (float)_y;
        *z = (float)_z;
    }
    return result;

};
