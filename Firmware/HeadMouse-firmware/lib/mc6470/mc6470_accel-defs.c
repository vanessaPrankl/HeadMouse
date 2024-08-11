/*
 * This is a register map for the mCube MC6470 6-axis eCompass.
 * Copyright (C) 2021  eResearch, James Cook University
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
 * 
 * Excluded from this copyright notice is the cited material from the following sources:
 * 
 * MC6470_Datasheet:
 *     name: MC6470_Datasheet
 *     url: https://mcubemems.com/product/mc6470-6-axis-ecompass/
 *     pdf: https://mcubemems.com/wp-content/uploads/2020/03/MC6470-Datasheet-APS-048-0033v1.7-1.pdf
 * 
 * mCube_mc6470_mcu_driver:
 *     name: mCube_mc6470_mcu_driver
 *     url: https://github.com/mcubemems/mCube_mc6470_mc7030_mcu_driver
 * 
 * Which remains the property of mCubic
 */

#include "mc6470_accel-defs.h"


MC6470_reg_value MC6470_ACCEL_validate_register_write(MC6470_reg_addr register_address, MC6470_reg_value value)
{
    switch(register_address)
    {
        case MC6470_ACCEL_MODE_ADDR:
        {
            return (((~MC6470_ACCEL_MODE_ADDR_VALIDATION_MASK & value) | (MC6470_ACCEL_MODE_ADDR_VALIDATION_MASK & MC6470_ACCEL_MODE_ADDR_VALIDATION_VALUE)));
        }break;
        case MC6470_ACCEL_HWID_ADDR:
        {
            return (((~MC6470_ACCEL_HWID_ADDR_VALIDATION_MASK & value) | (MC6470_ACCEL_HWID_ADDR_VALIDATION_MASK & MC6470_ACCEL_HWID_ADDR_VALIDATION_VALUE)));
        }break;
        case MC6470_ACCEL_OUTCFG_ADDR:
        {
            return (((~MC6470_ACCEL_OUTCFG_ADDR_VALIDATION_MASK & value) | (MC6470_ACCEL_OUTCFG_ADDR_VALIDATION_MASK & MC6470_ACCEL_OUTCFG_ADDR_VALIDATION_VALUE)));
        }break;
        default:break;
    }
    return value;
}

