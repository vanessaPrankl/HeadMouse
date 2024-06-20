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

#include "mc6470_accel.h"

uint32_t MC6470_Accel_Init(struct MC6470_Dev_t *dev)
{
    RETURN_ERROR_IF_NULL(dev);
    return MC6470_Status_OK;
};

uint32_t MC6470_Accel_ChipIDs(struct MC6470_Dev_t *dev, bool *found)
{
    RETURN_ERROR_IF_NULL(dev);
    uint8_t PCODE = 0;
    uint8_t HwId = 0;
    uint32_t result = MC6470_Accel_I2C_Read(dev, MC6470_ACCEL_PCODE_ADDR, &PCODE, sizeof(PCODE));
    result |= MC6470_Accel_I2C_Read(dev, MC6470_ACCEL_HWID_ADDR, &HwId, sizeof(HwId));
    PCODE = (PCODE & 0xF1); // https://github.com/jcu-eresearch/mCube_mc6470_mcu_driver/blob/d8998104a6445013b7298a5279dd96bbee8c91c3/MC7XXX_MCU_1.0.0/sensor/src/accel/m_drv_mc3xxx.c#L170-L182
    *found = (PCODE == MC6470_ACCEL_PCODE_3216) && (HwId == MC6470_ACCEL_HWID_MC3216);
    
    return result;  

};

uint32_t MC6470_Accel_get_Rate(struct MC6470_Dev_t *dev, MC6470_ACCEL_SRTFR_RATE_e *rate)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_SRTFR_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        *rate = MC6470_ACCEL_SRTFR_RATE_GET(current);
    }
    return result;
};

uint32_t MC6470_Accel_set_Rate(struct MC6470_Dev_t *dev, MC6470_ACCEL_SRTFR_RATE_e rate)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_SRTFR_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Standby);
    result |= MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_ACCEL_SRTFR_RATE_SET(current, rate);
        current = MC6470_ACCEL_validate_register_write(reg_addr, current);
        result |= MC6470_Accel_I2C_Write(dev, reg_addr, &current, sizeof(current));
        result |= MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Wake);
    };
    return result;
};

uint32_t MC6470_Accel_get_Range(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RANGE_e *range)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        if(range != NULL)
        {
            *range = MC6470_ACCEL_OUTCFG_RANGE_GET(current);
        }
        dev->accel_range = MC6470_ACCEL_OUTCFG_RANGE_GET(current);
    }
    return result;

};

uint32_t MC6470_Accel_set_Range(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RANGE_e range)
{
    RETURN_ERROR_IF_NULL(dev);
    
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Standby);
    result |= MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_ACCEL_OUTCFG_RANGE_SET(current, range);
        current = MC6470_ACCEL_validate_register_write(reg_addr, current);
        result = MC6470_Accel_I2C_Write(dev, reg_addr, &current, sizeof(current));
        if(!MC6470_IS_ERROR(result))
        {
            dev->accel_range = range;
        }        
        result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Wake);
    };
    return result;
};

uint32_t MC6470_Accel_get_Resolution(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RES_e *resolution)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        if(resolution != NULL)
        {
            *resolution = MC6470_ACCEL_OUTCFG_RES_GET(current);
        }
        dev->accel_resolution = MC6470_ACCEL_OUTCFG_RES_GET(current);
    }
    return result;
};

uint32_t MC6470_Accel_set_Resolution(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RES_e resolution)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Standby);
    result |= MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_ACCEL_OUTCFG_RES_SET(current, resolution);
        current = MC6470_ACCEL_validate_register_write(reg_addr, current);
        result = MC6470_Accel_I2C_Write(dev, reg_addr, &current, sizeof(current));
        if(!MC6470_IS_ERROR(result))
        {
            dev->accel_resolution = resolution;
        }        
        result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Wake);
    };
    return result;
};

uint32_t MC6470_Accel_get_Range_and_Resolution(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RANGE_e *range, MC6470_ACCEL_OUTCFG_RES_e *resolution)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result =  MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        if(resolution != NULL)
        {
            *resolution = MC6470_ACCEL_OUTCFG_RES_GET(current);
        }
        dev->accel_resolution = MC6470_ACCEL_OUTCFG_RES_GET(current);
        if(range != NULL)
        {
            *range = MC6470_ACCEL_OUTCFG_RANGE_GET(current);
        }
        dev->accel_range = MC6470_ACCEL_OUTCFG_RANGE_GET(current);        
    }
    return result;
};

uint32_t MC6470_Accel_set_Range_and_Resolution(struct MC6470_Dev_t *dev, MC6470_ACCEL_OUTCFG_RANGE_e range, MC6470_ACCEL_OUTCFG_RES_e resolution)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_OUTCFG_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Standby);
    current = MC6470_ACCEL_OUTCFG_RANGE_SET(current, range);
    current = MC6470_ACCEL_OUTCFG_RES_SET(current, resolution);
    current = MC6470_ACCEL_validate_register_write(reg_addr, current);
    result |= MC6470_Accel_I2C_Write(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        dev->accel_range = range;
        dev->accel_resolution = resolution;
    }
    result = MC6470_Accel_set_OperationState(dev, MC6470_ACCEL_MODE_OPCON_Wake);
    return result;
};

uint32_t MC6470_Accel_set_OperationState(struct MC6470_Dev_t *dev, MC6470_ACCEL_MODE_OPCON_e state)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_MODE_ADDR;
    MC6470_reg_value current = 0;
    uint32_t result = MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_ACCEL_MODE_OPCON_SET(current, state);
        current = MC6470_ACCEL_validate_register_write(reg_addr, current);
        result = MC6470_Accel_I2C_Write(dev, reg_addr, &current, sizeof(current));
        if(state == MC6470_ACCEL_MODE_OPCON_Wake){MC6470_delay_us(20);}
        
        reg_addr = MC6470_ACCEL_OPSTAT_ADDR;
        MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    };
    return result;
};

uint32_t MC6470_Accel_hasData(struct MC6470_Dev_t *dev, bool *has_data)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_SR_ADDR;
    uint32_t result = 0;
    uint8_t current = 0;
    
    result |= MC6470_Accel_I2C_Read(dev, reg_addr, &current, sizeof(current));
    int v = MC6470_ACCEL_SR_ACQ_INT_GET(current);
    if(!MC6470_IS_ERROR(result))
    {
        *has_data = v;
    }else{
        *has_data = false;
    }
    return result;
};

uint32_t MC6470_Accel_getData(struct MC6470_Dev_t *dev, float *x, float *y, float *z)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_ACCEL_XOUT_EX_L_ADDR;
    uint8_t data[6] = {0};
    uint32_t result = MC6470_Accel_I2C_Read(dev, reg_addr, data, sizeof(data));
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


uint32_t MC6470_Accel_I2C_Write(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    RETURN_ERROR_IF_NULL(dev);
    return MC6470_I2C_Write(dev, dev->accel_address, reg_address, buffer, buffer_length);
};

uint32_t MC6470_Accel_I2C_Read(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    RETURN_ERROR_IF_NULL(dev);
    return MC6470_I2C_Read(dev, dev->accel_address, reg_address, buffer, buffer_length);
};

