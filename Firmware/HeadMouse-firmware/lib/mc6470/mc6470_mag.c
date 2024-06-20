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

#include "mc6470_mag.h"

uint32_t MC6470_Mag_Init(struct MC6470_Dev_t *dev)
{
    return MC6470_Status_OK;
}

uint32_t MC6470_Mag_I2C_Write(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    RETURN_ERROR_IF_NULL(dev); 
    return MC6470_I2C_Write(dev, dev->mag_address, reg_address, buffer, buffer_length);
};

uint32_t MC6470_Mag_I2C_Read(struct MC6470_Dev_t *dev, MC6470_reg_addr reg_address, uint8_t *buffer, size_t buffer_length)
{
    RETURN_ERROR_IF_NULL(dev);
    return MC6470_I2C_Read(dev, dev->mag_address, reg_address, buffer, buffer_length);
};

uint32_t MC6470_Mag_ChipIDs(struct MC6470_Dev_t *dev, bool *found)
{
    RETURN_ERROR_IF_NULL(dev);
    uint8_t who_am_i = 0;
    uint32_t result = MC6470_Mag_I2C_Read(dev, MC6470_MAG_WHO_AM_I_ADDR, &who_am_i, sizeof(who_am_i));
    *found = (bool)who_am_i;
    
    return result;  
};

// Active or standby
uint32_t MC6470_Mag_set_Power_Mode(struct MC6470_Dev_t *dev, MC6470_MAG_CTRL_1_PC_e power_mode){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_1_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_1_PC_SET(current, power_mode);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}  

 // Normal Mode or force mode
uint32_t MC6470_Mag_set_Operation_Mode(struct MC6470_Dev_t *dev, MC6470_MAG_CTRL_1_FS_e mode){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_1_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_1_FS_SET(current, mode);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}

// measurement data rate
uint32_t MC6470_Mag_set_Data_Rate(struct MC6470_Dev_t *dev, MC6470_MAG_CTRL_1_ODR_e data_rate){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_1_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_1_ODR_SET(current, data_rate);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}

 // Set dynamic data output range (14 or 15 bit sigend int)
uint32_t MC6470_Mag_set_Data_Range(struct MC6470_Dev_t *dev, MC6470_MAG_CTRL_4_RS_e data_range){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_4_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_4_RS_SET(current, data_range);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}


// Configure interrupt pin for data ready signaling
uint32_t MC6470_Mag_set_ITR_Enable(struct MC6470_Dev_t *dev, MC6470_MAG_CTRL_2_DEN_e enable){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_2_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_2_DEN_SET(current, enable);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}

// Start single shot measurement in forced mode.
uint32_t MC6470_Mag_Start_Forced_Measurement(struct MC6470_Dev_t *dev){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_3_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_3_FRC_SET(current, MC6470_MAG_CTRL_3_FRC_Start);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}

static uint32_t MC6470_Mag_Temp_hasData(struct MC6470_Dev_t *dev, bool *has_data)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_3_ADDR;
    uint32_t result = 0;
    uint8_t current = 0;
    
    result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    MC6470_MAG_CTRL_3_TCS_e _hasData = MC6470_MAG_CTRL_3_TCS_GET(current);
    if(MC6470_IS_ERROR(result)) return result;
    
    if(_hasData == MC6470_MAG_CTRL_3_OCL_Default){
        *has_data = true;
    } else{
        *has_data = false;
    }
    return result;
}

// Start temperature measurment. Device must be in active mode and force state
uint32_t MC6470_Mag_Get_Temperature(struct MC6470_Dev_t *dev, int8_t *temp){
    RETURN_ERROR_IF_NULL(dev);
    uint8_t current = 0;
    uint8_t _temp = 0;

    /* Check if device is in force- or normal-state */
    uint32_t result = MC6470_Mag_I2C_Read(dev, MC6470_MAG_CTRL_1_ADDR, &current, sizeof(current));
    if(MC6470_IS_ERROR(result)) return result;
    MC6470_MAG_CTRL_1_FS_e state = MC6470_MAG_CTRL_1_FS_GET(current);

    /* Switch into force state */
    if(state == MC6470_MAG_CTRL_1_FS_Normal){
        result = MC6470_Mag_set_Operation_Mode(dev, MC6470_MAG_CTRL_1_FS_Force);
        if(MC6470_IS_ERROR(result)) return result;    
    }

    /* Start tempearture measurement */
    result = MC6470_Mag_I2C_Read(dev, MC6470_MAG_CTRL_3_ADDR, &current, sizeof(current));
    if(MC6470_IS_ERROR(result)) return result;
    
    current = MC6470_MAG_CTRL_3_TCS_SET(current, MC6470_MAG_CTRL_3_TCS_Start);
    result = MC6470_Mag_I2C_Write(dev, MC6470_MAG_CTRL_3_ADDR, &current, sizeof(current));       
    if(MC6470_IS_ERROR(result)) return result;

    /* Wait for measurement to finish */
    bool _has_data = false;
    while(_has_data == false){
        result = MC6470_Mag_Temp_hasData(dev, &_has_data);
        if(MC6470_IS_ERROR(result)) return result;
    }

    /* Read new temperature after measurement has finished*/
    result = MC6470_Mag_I2C_Read(dev, MC6470_MAG_TEMPERATURE_ADDR, &_temp, sizeof(current));
    if(MC6470_IS_ERROR(result)) return result;
    *temp = _temp;

    /* Return to old state which was active before temp measurement */
    if(state == MC6470_MAG_CTRL_1_FS_Normal){
        result = MC6470_Mag_set_Operation_Mode(dev, MC6470_MAG_CTRL_1_FS_Normal);
        if(MC6470_IS_ERROR(result)) return result;    
    }
    return result;
}

// Calibrate measurment offset. Device must be in active mode and force state.
uint32_t MC6470_Mag_Calibrate_Offset(struct MC6470_Dev_t *dev){
    RETURN_ERROR_IF_NULL(dev); 
    MC6470_reg_addr reg_addr = MC6470_MAG_CTRL_3_ADDR;
    MC6470_reg_value current = 0;
    
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    if(!MC6470_IS_ERROR(result))
    {
        current = MC6470_MAG_CTRL_3_OCL_SET(current, MC6470_MAG_CTRL_3_OCL_Start);
        result = MC6470_Mag_I2C_Write(dev, reg_addr, &current, sizeof(current));       
    } 
    return result;
}



uint32_t MC6470_Mag_hasData(struct MC6470_Dev_t *dev, bool *has_data)
{
    RETURN_ERROR_IF_NULL(dev);
    MC6470_reg_addr reg_addr = MC6470_MAG_STATUS_ADDR;
    uint32_t result = 0;
    uint8_t current = 0;
    
    result |= MC6470_Mag_I2C_Read(dev, reg_addr, &current, sizeof(current));
    int v = MC6470_MAG_STATUS_DRDY_GET(current);
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
    MC6470_reg_addr reg_addr = MC6470_MAG_X_AXIS_LSB_ADDR;
    uint8_t data[6] = {0};
    uint32_t result = MC6470_Mag_I2C_Read(dev, reg_addr, data, sizeof(data));
    bool has_data = false;
    short _x = 0;
    short _y = 0;
    short _z = 0;
    while(!has_data)
    {
        MC6470_Mag_hasData(dev, &has_data);
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
