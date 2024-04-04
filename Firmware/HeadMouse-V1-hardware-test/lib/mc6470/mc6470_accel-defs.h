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

#ifndef __MC6470_ACCEL_DEFS_H__
#define __MC6470_ACCEL_DEFS_H__

#include "mc6470_defs.h"

#define MC6470_ACCEL_ADDRESS_7_BITS_0                  0x4C /**< Pin A5 set to GND */
#define MC6470_ACCEL_ADDRESS_7_BITS_1                  0x6C /**< Pin A5 set to VDD */

#define MC6470_ACCEL_SR_ADDR                           0x03 /**< This register contains the flag/event bits for tap detection and sample acquisition.  [MC6470_Datasheet] */
#define MC6470_ACCEL_OPSTAT_ADDR                       0x04 /**< The device status register reports various conditions of the sensor circuitry.  [MC6470_Datasheet] */
#define MC6470_ACCEL_INTEN_ADDR                        0x06 /**< The interrupt enable register allows the flag bits for specific TAP and sample events to also trigger a transition of the external INTA pin.  [MC6470_Datasheet] */
#define MC6470_ACCEL_MODE_ADDR                         0x07 /**< The MODE register controls the active operating state of the device. This register can be written from either operational state (STANDBY or WAKE).  [MC6470_Datasheet] */
#define MC6470_ACCEL_SRTFR_ADDR                        0x08 /**< This register sets the sampling output data rate (ODR) for sensor. The upper 4 bit control functions related to tap hardware.  [MC6470_Datasheet] */
#define MC6470_ACCEL_TAPEN_ADDR                        0x09 /**< This register allows the enabling and disabling of tap detection for axes and direction. Bit 7 disables tap detection completely. Bit 6, switches the feature controlled by registers 0xA, 0xB, and 0xC. When bit 6 is ‘0’, the tap duration and quiet parameters are accessed in 0xA to 0xC, and when ‘1’ the tap detection threshold is accessed.  [MC6470_Datasheet] */
#define MC6470_ACCEL_TTTRX_ADDR                        0x0A /**< This register allows the control (on the X axis) of both the tap duration settings and tap threshold settings, depending upon the setting of the THRDUR bit (bit 6) in the TAPEN register (0x09).  [MC6470_Datasheet] */
#define MC6470_ACCEL_TTTRY_ADDR                        0x0B /**< This register allows the control (on the Y axis) of both the tap duration settings and tap threshold settings, depending upon the setting of the THRDUR bit (bit 6) in the TAPEN register (0x09).  [MC6470_Datasheet] */
#define MC6470_ACCEL_TTTRZ_ADDR                        0x0C /**< This register allows the control (on the Z axis) of both the tap duration settings and tap threshold settings, depending upon the setting of the THRDUR bit (bit 6) in the TAPEN register (0x09).  [MC6470_Datasheet] */

/**
The measurements from sensors for the 3-axes are available in these 3 registers. The most-significant
bit of the value is the sign bit, and is sign extended to the higher bits. Note that all 3 axes are sampled
and updated simultaneously. If an I2C burst read operation reads past register address 0x12 the
internal address pointer “wraps” to address 0x03 and the contents of the SR: Status Register are
returned. This allows application software to burst read the contents of the six extended registers and
relevant device state registers in a single I2C read cycle.
Once an I2C start bit has been recognized by the sensor, registers will not be updated until an I2C stop
bit has occurred. Therefore, if software desires to read the low and high byte registers ‘atomically’,
knowing that the values have not been changed, it should do so by issuing a start bit, reading one
register, then reading the other register then issuing a stop bit. Note that all 6 registers may be read in
one burst with the same effect.
 */ // [MC6470_Datasheet] 

#define MC6470_ACCEL_XOUT_EX_L_ADDR                    0x0D /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_XOUT_EX_H_ADDR                    0x0E /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_YOUT_EX_L_ADDR                    0x0F /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_YOUT_EX_H_ADDR                    0x10 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_ZOUT_EX_L_ADDR                    0x11 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_ZOUT_EX_H_ADDR                    0x12 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_HWID_ADDR                         0x18 /**< This register seems to hold some kind of Hardware ID. It is not documented in the datasheet but used https://github.com/jcu-eresearch/mCube_mc6470_mcu_driver/blob/d8998104a6445013b7298a5279dd96bbee8c91c3/MC7XXX_MCU_1.0.0/sensor/src/accel/m_drv_mc3xxx.c#L130  [mCube_mc6470_mcu_driver] */
#define MC6470_ACCEL_OUTCFG_ADDR                       0x20 /**< This register can be used to set the range and resolution of the accelerometer measurements.  [MC6470_Datasheet] */

/**
This register contains a signed 2’s complement 15-bit value applied as an offset adjustment to the
output of the sensor values, prior to being sent to the OUT_EX registers. The Power-On-Reset value
for each chip is unique and is set as part of factory calibration. If necessary, this value can be
overwritten by software.

NOTE: When modifying these registers with new gain or offset values, software should perform
a read-modify-write type of access to ensure that unrelated bits do not get changed
inadvertently.
 */ // [MC6470_Datasheet] 

#define MC6470_ACCEL_XOFFL_ADDR                        0x21 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_XOFFH_ADDR                        0x22 /**< .  [MC6470_Datasheet] */

/**
This register contains a signed 2’s complement 15-bit value applied as an offset adjustment to the
output of the sensor values, prior to being sent to the OUT_EX registers. The Power-On-Reset value
for each chip is unique and is set as part of factory calibration. If necessary, this value can be
overwritten by software.

NOTE: When modifying these registers with new gain or offset values, software should perform
a read-modify-write type of access to ensure that unrelated bits do not get changed
inadvertently.
 */ // [MC6470_Datasheet] 

#define MC6470_ACCEL_YOFFL_ADDR                        0x23 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_YOFFH_ADDR                        0x24 /**< .  [MC6470_Datasheet] */

/**
This register contains a signed 2’s complement 15-bit value applied as an offset adjustment to the
output of the sensor values, prior to being sent to the OUT_EX registers. The Power-On-Reset value
for each chip is unique and is set as part of factory calibration. If necessary, this value can be
overwritten by software.

NOTE: When modifying these registers with new gain or offset values, software should perform
a read-modify-write type of access to ensure that unrelated bits do not get changed
inadvertently.
 */ // [MC6470_Datasheet] 

#define MC6470_ACCEL_ZOFFL_ADDR                        0x25 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_ZOFFH_ADDR                        0x26 /**< .  [MC6470_Datasheet] */
#define MC6470_ACCEL_XGAIN_ADDR                        0x27 /**< The 8 least significant bits of the X-Axis Gain. The most significant bit is in 0x22(XOFFH) [7:7].  [MC6470_Datasheet] */
#define MC6470_ACCEL_YGAIN_ADDR                        0x28 /**< The 8 least significant bits of the Y-Axis Gain. The most significant bit is in 0x24(YOFFH) [7:7].  [MC6470_Datasheet] */
#define MC6470_ACCEL_ZGAIN_ADDR                        0x29 /**< The 8 least significant bits of the Z-Axis Gain. The most significant bit is in 0x26(ZOFFH) [7:7].  [MC6470_Datasheet] */
#define MC6470_ACCEL_PCODE_ADDR                        0x3B /**< This register returns a value specific to the part number of this mCube device.  [MC6470_Datasheet] */

#define MC6470_ACCEL_SR_ACQ_INT_POS                    7
#define MC6470_ACCEL_SR_ACQ_INT_MASK                   (0b00000001 << MC6470_ACCEL_SR_ACQ_INT_POS) /**< [7..7] Sample has been acquired, flag bit is set in polling mode or interrupt mode. This bit cannot be disabled and is always set be hardware when a sample is ready. The host must poll at the sample rate of faster to see this bit transition */
#define MC6470_ACCEL_SR_ACQ_INT_GET(R_V)               ((R_V & MC6470_ACCEL_SR_ACQ_INT_MASK) >> MC6470_ACCEL_SR_ACQ_INT_POS)
#define MC6470_ACCEL_SR_ACQ_INT_SET(R_V, F_V)          ((R_V & (~MC6470_ACCEL_SR_ACQ_INT_MASK)) | ((F_V & (MC6470_ACCEL_SR_ACQ_INT_MASK >> MC6470_ACCEL_SR_ACQ_INT_POS)) << MC6470_ACCEL_SR_ACQ_INT_POS))
#define MC6470_ACCEL_SR_TAP_ZN_POS                     5
#define MC6470_ACCEL_SR_TAP_ZN_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_ZN_POS) /**< [5..5] Negative Z-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_ZN_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_ZN_MASK) >> MC6470_ACCEL_SR_TAP_ZN_POS)
#define MC6470_ACCEL_SR_TAP_ZN_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_ZN_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_ZN_MASK >> MC6470_ACCEL_SR_TAP_ZN_POS)) << MC6470_ACCEL_SR_TAP_ZN_POS))
#define MC6470_ACCEL_SR_TAP_ZP_POS                     4
#define MC6470_ACCEL_SR_TAP_ZP_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_ZP_POS) /**< [4..4] Positive Z-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_ZP_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_ZP_MASK) >> MC6470_ACCEL_SR_TAP_ZP_POS)
#define MC6470_ACCEL_SR_TAP_ZP_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_ZP_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_ZP_MASK >> MC6470_ACCEL_SR_TAP_ZP_POS)) << MC6470_ACCEL_SR_TAP_ZP_POS))
#define MC6470_ACCEL_SR_TAP_YN_POS                     3
#define MC6470_ACCEL_SR_TAP_YN_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_YN_POS) /**< [3..3] Negative Y-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_YN_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_YN_MASK) >> MC6470_ACCEL_SR_TAP_YN_POS)
#define MC6470_ACCEL_SR_TAP_YN_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_YN_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_YN_MASK >> MC6470_ACCEL_SR_TAP_YN_POS)) << MC6470_ACCEL_SR_TAP_YN_POS))
#define MC6470_ACCEL_SR_TAP_YP_POS                     2
#define MC6470_ACCEL_SR_TAP_YP_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_YP_POS) /**< [2..2] Positive X-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_YP_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_YP_MASK) >> MC6470_ACCEL_SR_TAP_YP_POS)
#define MC6470_ACCEL_SR_TAP_YP_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_YP_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_YP_MASK >> MC6470_ACCEL_SR_TAP_YP_POS)) << MC6470_ACCEL_SR_TAP_YP_POS))
#define MC6470_ACCEL_SR_TAP_XN_POS                     1
#define MC6470_ACCEL_SR_TAP_XN_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_XN_POS) /**< [1..1] Negative X-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_XN_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_XN_MASK) >> MC6470_ACCEL_SR_TAP_XN_POS)
#define MC6470_ACCEL_SR_TAP_XN_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_XN_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_XN_MASK >> MC6470_ACCEL_SR_TAP_XN_POS)) << MC6470_ACCEL_SR_TAP_XN_POS))
#define MC6470_ACCEL_SR_TAP_XP_POS                     0
#define MC6470_ACCEL_SR_TAP_XP_MASK                    (0b00000001 << MC6470_ACCEL_SR_TAP_XP_POS) /**< [0..0] Positive X-axis TAP detected, flag is set in polling mode or interrupt mode. */
#define MC6470_ACCEL_SR_TAP_XP_GET(R_V)                ((R_V & MC6470_ACCEL_SR_TAP_XP_MASK) >> MC6470_ACCEL_SR_TAP_XP_POS)
#define MC6470_ACCEL_SR_TAP_XP_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_SR_TAP_XP_MASK)) | ((F_V & (MC6470_ACCEL_SR_TAP_XP_MASK >> MC6470_ACCEL_SR_TAP_XP_POS)) << MC6470_ACCEL_SR_TAP_XP_POS))
#define MC6470_ACCEL_OPSTAT_OTPA_POS                   7
#define MC6470_ACCEL_OPSTAT_OTPA_MASK                  (0b00000001 << MC6470_ACCEL_OPSTAT_OTPA_POS) /**< [7..7] One-time Programming (OTP) activity status */
#define MC6470_ACCEL_OPSTAT_OTPA_GET(R_V)              ((R_V & MC6470_ACCEL_OPSTAT_OTPA_MASK) >> MC6470_ACCEL_OPSTAT_OTPA_POS)
#define MC6470_ACCEL_OPSTAT_OTPA_SET(R_V, F_V)         ((R_V & (~MC6470_ACCEL_OPSTAT_OTPA_MASK)) | ((F_V & (MC6470_ACCEL_OPSTAT_OTPA_MASK >> MC6470_ACCEL_OPSTAT_OTPA_POS)) << MC6470_ACCEL_OPSTAT_OTPA_POS))
#define MC6470_ACCEL_OPSTAT_I2C_WDT_POS                4
#define MC6470_ACCEL_OPSTAT_I2C_WDT_MASK               (0b00000001 << MC6470_ACCEL_OPSTAT_I2C_WDT_POS) /**< [4..4] I2C watchdog timeout */
#define MC6470_ACCEL_OPSTAT_I2C_WDT_GET(R_V)           ((R_V & MC6470_ACCEL_OPSTAT_I2C_WDT_MASK) >> MC6470_ACCEL_OPSTAT_I2C_WDT_POS)
#define MC6470_ACCEL_OPSTAT_I2C_WDT_SET(R_V, F_V)      ((R_V & (~MC6470_ACCEL_OPSTAT_I2C_WDT_MASK)) | ((F_V & (MC6470_ACCEL_OPSTAT_I2C_WDT_MASK >> MC6470_ACCEL_OPSTAT_I2C_WDT_POS)) << MC6470_ACCEL_OPSTAT_I2C_WDT_POS))
#define MC6470_ACCEL_OPSTAT_OPSTAT_POS                 0
#define MC6470_ACCEL_OPSTAT_OPSTAT_MASK                (0b00000011 << MC6470_ACCEL_OPSTAT_OPSTAT_POS) /**< [1..0] Sampling State Register Status, Wait State Register Status */
#define MC6470_ACCEL_OPSTAT_OPSTAT_GET(R_V)            ((R_V & MC6470_ACCEL_OPSTAT_OPSTAT_MASK) >> MC6470_ACCEL_OPSTAT_OPSTAT_POS)
#define MC6470_ACCEL_OPSTAT_OPSTAT_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_OPSTAT_OPSTAT_MASK)) | ((F_V & (MC6470_ACCEL_OPSTAT_OPSTAT_MASK >> MC6470_ACCEL_OPSTAT_OPSTAT_POS)) << MC6470_ACCEL_OPSTAT_OPSTAT_POS))
#define MC6470_ACCEL_INTEN_ACQ_INT_EN_POS              7
#define MC6470_ACCEL_INTEN_ACQ_INT_EN_MASK             (0b00000001 << MC6470_ACCEL_INTEN_ACQ_INT_EN_POS) /**< [7..7] Enable Interrupts */
#define MC6470_ACCEL_INTEN_ACQ_INT_EN_GET(R_V)         ((R_V & MC6470_ACCEL_INTEN_ACQ_INT_EN_MASK) >> MC6470_ACCEL_INTEN_ACQ_INT_EN_POS)
#define MC6470_ACCEL_INTEN_ACQ_INT_EN_SET(R_V, F_V)    ((R_V & (~MC6470_ACCEL_INTEN_ACQ_INT_EN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_ACQ_INT_EN_MASK >> MC6470_ACCEL_INTEN_ACQ_INT_EN_POS)) << MC6470_ACCEL_INTEN_ACQ_INT_EN_POS))
#define MC6470_ACCEL_INTEN_TIZNEN_POS                  5
#define MC6470_ACCEL_INTEN_TIZNEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIZNEN_POS) /**< [5..5] Negative Z-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIZNEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIZNEN_MASK) >> MC6470_ACCEL_INTEN_TIZNEN_POS)
#define MC6470_ACCEL_INTEN_TIZNEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIZNEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIZNEN_MASK >> MC6470_ACCEL_INTEN_TIZNEN_POS)) << MC6470_ACCEL_INTEN_TIZNEN_POS))
#define MC6470_ACCEL_INTEN_TIZPEN_POS                  4
#define MC6470_ACCEL_INTEN_TIZPEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIZPEN_POS) /**< [4..4] Positive Z-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIZPEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIZPEN_MASK) >> MC6470_ACCEL_INTEN_TIZPEN_POS)
#define MC6470_ACCEL_INTEN_TIZPEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIZPEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIZPEN_MASK >> MC6470_ACCEL_INTEN_TIZPEN_POS)) << MC6470_ACCEL_INTEN_TIZPEN_POS))
#define MC6470_ACCEL_INTEN_TIYNEN_POS                  3
#define MC6470_ACCEL_INTEN_TIYNEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIYNEN_POS) /**< [3..3] Negative Y-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIYNEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIYNEN_MASK) >> MC6470_ACCEL_INTEN_TIYNEN_POS)
#define MC6470_ACCEL_INTEN_TIYNEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIYNEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIYNEN_MASK >> MC6470_ACCEL_INTEN_TIYNEN_POS)) << MC6470_ACCEL_INTEN_TIYNEN_POS))
#define MC6470_ACCEL_INTEN_TIYPEN_POS                  2
#define MC6470_ACCEL_INTEN_TIYPEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIYPEN_POS) /**< [2..2] Positive Y-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIYPEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIYPEN_MASK) >> MC6470_ACCEL_INTEN_TIYPEN_POS)
#define MC6470_ACCEL_INTEN_TIYPEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIYPEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIYPEN_MASK >> MC6470_ACCEL_INTEN_TIYPEN_POS)) << MC6470_ACCEL_INTEN_TIYPEN_POS))
#define MC6470_ACCEL_INTEN_TIZXEN_POS                  1
#define MC6470_ACCEL_INTEN_TIZXEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIZXEN_POS) /**< [1..1] Negative X-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIZXEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIZXEN_MASK) >> MC6470_ACCEL_INTEN_TIZXEN_POS)
#define MC6470_ACCEL_INTEN_TIZXEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIZXEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIZXEN_MASK >> MC6470_ACCEL_INTEN_TIZXEN_POS)) << MC6470_ACCEL_INTEN_TIZXEN_POS))
#define MC6470_ACCEL_INTEN_TIXPEN_POS                  0
#define MC6470_ACCEL_INTEN_TIXPEN_MASK                 (0b00000001 << MC6470_ACCEL_INTEN_TIXPEN_POS) /**< [0..0] Positive X-axis TAP interrupt enable */
#define MC6470_ACCEL_INTEN_TIXPEN_GET(R_V)             ((R_V & MC6470_ACCEL_INTEN_TIXPEN_MASK) >> MC6470_ACCEL_INTEN_TIXPEN_POS)
#define MC6470_ACCEL_INTEN_TIXPEN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_INTEN_TIXPEN_MASK)) | ((F_V & (MC6470_ACCEL_INTEN_TIXPEN_MASK >> MC6470_ACCEL_INTEN_TIXPEN_POS)) << MC6470_ACCEL_INTEN_TIXPEN_POS))
#define MC6470_ACCEL_MODE_IAH_POS                      7
#define MC6470_ACCEL_MODE_IAH_MASK                     (0b00000001 << MC6470_ACCEL_MODE_IAH_POS) /**< [7..7] Sets weather the Interrupt pin is Active High or Active Low */
#define MC6470_ACCEL_MODE_IAH_GET(R_V)                 ((R_V & MC6470_ACCEL_MODE_IAH_MASK) >> MC6470_ACCEL_MODE_IAH_POS)
#define MC6470_ACCEL_MODE_IAH_SET(R_V, F_V)            ((R_V & (~MC6470_ACCEL_MODE_IAH_MASK)) | ((F_V & (MC6470_ACCEL_MODE_IAH_MASK >> MC6470_ACCEL_MODE_IAH_POS)) << MC6470_ACCEL_MODE_IAH_POS))
#define MC6470_ACCEL_MODE_IPP_POS                      6
#define MC6470_ACCEL_MODE_IPP_MASK                     (0b00000001 << MC6470_ACCEL_MODE_IPP_POS) /**< [6..6] Sets weather the Interrupt pin Push Pull or Open Drain */
#define MC6470_ACCEL_MODE_IPP_GET(R_V)                 ((R_V & MC6470_ACCEL_MODE_IPP_MASK) >> MC6470_ACCEL_MODE_IPP_POS)
#define MC6470_ACCEL_MODE_IPP_SET(R_V, F_V)            ((R_V & (~MC6470_ACCEL_MODE_IPP_MASK)) | ((F_V & (MC6470_ACCEL_MODE_IPP_MASK >> MC6470_ACCEL_MODE_IPP_POS)) << MC6470_ACCEL_MODE_IPP_POS))
#define MC6470_ACCEL_MODE_I2C_WDT_POS_POS              5
#define MC6470_ACCEL_MODE_I2C_WDT_POS_MASK             (0b00000001 << MC6470_ACCEL_MODE_I2C_WDT_POS_POS) /**< [5..5] Enabled WDT for positive SCL stalls */
#define MC6470_ACCEL_MODE_I2C_WDT_POS_GET(R_V)         ((R_V & MC6470_ACCEL_MODE_I2C_WDT_POS_MASK) >> MC6470_ACCEL_MODE_I2C_WDT_POS_POS)
#define MC6470_ACCEL_MODE_I2C_WDT_POS_SET(R_V, F_V)    ((R_V & (~MC6470_ACCEL_MODE_I2C_WDT_POS_MASK)) | ((F_V & (MC6470_ACCEL_MODE_I2C_WDT_POS_MASK >> MC6470_ACCEL_MODE_I2C_WDT_POS_POS)) << MC6470_ACCEL_MODE_I2C_WDT_POS_POS))
#define MC6470_ACCEL_MODE_I2C_WDT_NEG_POS              4
#define MC6470_ACCEL_MODE_I2C_WDT_NEG_MASK             (0b00000001 << MC6470_ACCEL_MODE_I2C_WDT_NEG_POS) /**< [4..4] Enabled WDT for negative SCL stalls */
#define MC6470_ACCEL_MODE_I2C_WDT_NEG_GET(R_V)         ((R_V & MC6470_ACCEL_MODE_I2C_WDT_NEG_MASK) >> MC6470_ACCEL_MODE_I2C_WDT_NEG_POS)
#define MC6470_ACCEL_MODE_I2C_WDT_NEG_SET(R_V, F_V)    ((R_V & (~MC6470_ACCEL_MODE_I2C_WDT_NEG_MASK)) | ((F_V & (MC6470_ACCEL_MODE_I2C_WDT_NEG_MASK >> MC6470_ACCEL_MODE_I2C_WDT_NEG_POS)) << MC6470_ACCEL_MODE_I2C_WDT_NEG_POS))
#define MC6470_ACCEL_MODE_OPCON_POS                    0
#define MC6470_ACCEL_MODE_OPCON_MASK                   (0b00000011 << MC6470_ACCEL_MODE_OPCON_POS) /**< [1..0] Set Device Operational State. WAKE or STANDBY */
#define MC6470_ACCEL_MODE_OPCON_GET(R_V)               ((R_V & MC6470_ACCEL_MODE_OPCON_MASK) >> MC6470_ACCEL_MODE_OPCON_POS)
#define MC6470_ACCEL_MODE_OPCON_SET(R_V, F_V)          ((R_V & (~MC6470_ACCEL_MODE_OPCON_MASK)) | ((F_V & (MC6470_ACCEL_MODE_OPCON_MASK >> MC6470_ACCEL_MODE_OPCON_POS)) << MC6470_ACCEL_MODE_OPCON_POS))
#define MC6470_ACCEL_SRTFR_TAP_LATCH_POS               7
#define MC6470_ACCEL_SRTFR_TAP_LATCH_MASK              (0b00000001 << MC6470_ACCEL_SRTFR_TAP_LATCH_POS) /**< [7..7] Selects weather Multiple Taps or the First Tap is Latched. */
#define MC6470_ACCEL_SRTFR_TAP_LATCH_GET(R_V)          ((R_V & MC6470_ACCEL_SRTFR_TAP_LATCH_MASK) >> MC6470_ACCEL_SRTFR_TAP_LATCH_POS)
#define MC6470_ACCEL_SRTFR_TAP_LATCH_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_SRTFR_TAP_LATCH_MASK)) | ((F_V & (MC6470_ACCEL_SRTFR_TAP_LATCH_MASK >> MC6470_ACCEL_SRTFR_TAP_LATCH_POS)) << MC6470_ACCEL_SRTFR_TAP_LATCH_POS))
#define MC6470_ACCEL_SRTFR_FLIP_TAPZ_POS               6
#define MC6470_ACCEL_SRTFR_FLIP_TAPZ_MASK              (0b00000001 << MC6470_ACCEL_SRTFR_FLIP_TAPZ_POS) /**< [6..6] Flips the Z Tap Detection. */
#define MC6470_ACCEL_SRTFR_FLIP_TAPZ_GET(R_V)          ((R_V & MC6470_ACCEL_SRTFR_FLIP_TAPZ_MASK) >> MC6470_ACCEL_SRTFR_FLIP_TAPZ_POS)
#define MC6470_ACCEL_SRTFR_FLIP_TAPZ_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_SRTFR_FLIP_TAPZ_MASK)) | ((F_V & (MC6470_ACCEL_SRTFR_FLIP_TAPZ_MASK >> MC6470_ACCEL_SRTFR_FLIP_TAPZ_POS)) << MC6470_ACCEL_SRTFR_FLIP_TAPZ_POS))
#define MC6470_ACCEL_SRTFR_FLIP_TAPY_POS               5
#define MC6470_ACCEL_SRTFR_FLIP_TAPY_MASK              (0b00000001 << MC6470_ACCEL_SRTFR_FLIP_TAPY_POS) /**< [5..5] Flips the Y Tap Detection. */
#define MC6470_ACCEL_SRTFR_FLIP_TAPY_GET(R_V)          ((R_V & MC6470_ACCEL_SRTFR_FLIP_TAPY_MASK) >> MC6470_ACCEL_SRTFR_FLIP_TAPY_POS)
#define MC6470_ACCEL_SRTFR_FLIP_TAPY_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_SRTFR_FLIP_TAPY_MASK)) | ((F_V & (MC6470_ACCEL_SRTFR_FLIP_TAPY_MASK >> MC6470_ACCEL_SRTFR_FLIP_TAPY_POS)) << MC6470_ACCEL_SRTFR_FLIP_TAPY_POS))
#define MC6470_ACCEL_SRTFR_FLIP_TAPX_POS               4
#define MC6470_ACCEL_SRTFR_FLIP_TAPX_MASK              (0b00000001 << MC6470_ACCEL_SRTFR_FLIP_TAPX_POS) /**< [4..4] Flips the X Tap Detection. */
#define MC6470_ACCEL_SRTFR_FLIP_TAPX_GET(R_V)          ((R_V & MC6470_ACCEL_SRTFR_FLIP_TAPX_MASK) >> MC6470_ACCEL_SRTFR_FLIP_TAPX_POS)
#define MC6470_ACCEL_SRTFR_FLIP_TAPX_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_SRTFR_FLIP_TAPX_MASK)) | ((F_V & (MC6470_ACCEL_SRTFR_FLIP_TAPX_MASK >> MC6470_ACCEL_SRTFR_FLIP_TAPX_POS)) << MC6470_ACCEL_SRTFR_FLIP_TAPX_POS))
#define MC6470_ACCEL_SRTFR_RATE_POS                    0
#define MC6470_ACCEL_SRTFR_RATE_MASK                   (0b00001111 << MC6470_ACCEL_SRTFR_RATE_POS) /**< [3..0] Rate. */
#define MC6470_ACCEL_SRTFR_RATE_GET(R_V)               ((R_V & MC6470_ACCEL_SRTFR_RATE_MASK) >> MC6470_ACCEL_SRTFR_RATE_POS)
#define MC6470_ACCEL_SRTFR_RATE_SET(R_V, F_V)          ((R_V & (~MC6470_ACCEL_SRTFR_RATE_MASK)) | ((F_V & (MC6470_ACCEL_SRTFR_RATE_MASK >> MC6470_ACCEL_SRTFR_RATE_POS)) << MC6470_ACCEL_SRTFR_RATE_POS))
#define MC6470_ACCEL_TAPEN_TAP_EN_POS                  7
#define MC6470_ACCEL_TAPEN_TAP_EN_MASK                 (0b00000001 << MC6470_ACCEL_TAPEN_TAP_EN_POS) /**< [7..7] Enable Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAP_EN_GET(R_V)             ((R_V & MC6470_ACCEL_TAPEN_TAP_EN_MASK) >> MC6470_ACCEL_TAPEN_TAP_EN_POS)
#define MC6470_ACCEL_TAPEN_TAP_EN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_TAPEN_TAP_EN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAP_EN_MASK >> MC6470_ACCEL_TAPEN_TAP_EN_POS)) << MC6470_ACCEL_TAPEN_TAP_EN_POS))
#define MC6470_ACCEL_TAPEN_THRDUR_POS                  6
#define MC6470_ACCEL_TAPEN_THRDUR_MASK                 (0b00000001 << MC6470_ACCEL_TAPEN_THRDUR_POS) /**< [6..6] Configure weather the TTTRX, TTTRY, and TTTRZ registers hold tap duration and quite period or tap threshold settings. */
#define MC6470_ACCEL_TAPEN_THRDUR_GET(R_V)             ((R_V & MC6470_ACCEL_TAPEN_THRDUR_MASK) >> MC6470_ACCEL_TAPEN_THRDUR_POS)
#define MC6470_ACCEL_TAPEN_THRDUR_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_TAPEN_THRDUR_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_THRDUR_MASK >> MC6470_ACCEL_TAPEN_THRDUR_POS)) << MC6470_ACCEL_TAPEN_THRDUR_POS))
#define MC6470_ACCEL_TAPEN_TAPZNEN_POS                 5
#define MC6470_ACCEL_TAPEN_TAPZNEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPZNEN_POS) /**< [5..5] Enable negative Z Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPZNEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPZNEN_MASK) >> MC6470_ACCEL_TAPEN_TAPZNEN_POS)
#define MC6470_ACCEL_TAPEN_TAPZNEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPZNEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPZNEN_MASK >> MC6470_ACCEL_TAPEN_TAPZNEN_POS)) << MC6470_ACCEL_TAPEN_TAPZNEN_POS))
#define MC6470_ACCEL_TAPEN_TAPZPEN_POS                 4
#define MC6470_ACCEL_TAPEN_TAPZPEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPZPEN_POS) /**< [4..4] Enable positive Z Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPZPEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPZPEN_MASK) >> MC6470_ACCEL_TAPEN_TAPZPEN_POS)
#define MC6470_ACCEL_TAPEN_TAPZPEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPZPEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPZPEN_MASK >> MC6470_ACCEL_TAPEN_TAPZPEN_POS)) << MC6470_ACCEL_TAPEN_TAPZPEN_POS))
#define MC6470_ACCEL_TAPEN_TAPYNEN_POS                 3
#define MC6470_ACCEL_TAPEN_TAPYNEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPYNEN_POS) /**< [3..3] Enable negative Y Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPYNEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPYNEN_MASK) >> MC6470_ACCEL_TAPEN_TAPYNEN_POS)
#define MC6470_ACCEL_TAPEN_TAPYNEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPYNEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPYNEN_MASK >> MC6470_ACCEL_TAPEN_TAPYNEN_POS)) << MC6470_ACCEL_TAPEN_TAPYNEN_POS))
#define MC6470_ACCEL_TAPEN_TAPYPEN_POS                 2
#define MC6470_ACCEL_TAPEN_TAPYPEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPYPEN_POS) /**< [2..2] Enable positive Y Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPYPEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPYPEN_MASK) >> MC6470_ACCEL_TAPEN_TAPYPEN_POS)
#define MC6470_ACCEL_TAPEN_TAPYPEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPYPEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPYPEN_MASK >> MC6470_ACCEL_TAPEN_TAPYPEN_POS)) << MC6470_ACCEL_TAPEN_TAPYPEN_POS))
#define MC6470_ACCEL_TAPEN_TAPXNEN_POS                 1
#define MC6470_ACCEL_TAPEN_TAPXNEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPXNEN_POS) /**< [1..1] Enable negative X Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPXNEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPXNEN_MASK) >> MC6470_ACCEL_TAPEN_TAPXNEN_POS)
#define MC6470_ACCEL_TAPEN_TAPXNEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPXNEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPXNEN_MASK >> MC6470_ACCEL_TAPEN_TAPXNEN_POS)) << MC6470_ACCEL_TAPEN_TAPXNEN_POS))
#define MC6470_ACCEL_TAPEN_TAPXPEN_POS                 0
#define MC6470_ACCEL_TAPEN_TAPXPEN_MASK                (0b00000001 << MC6470_ACCEL_TAPEN_TAPXPEN_POS) /**< [0..0] Enable positive X Tap Detection. */
#define MC6470_ACCEL_TAPEN_TAPXPEN_GET(R_V)            ((R_V & MC6470_ACCEL_TAPEN_TAPXPEN_MASK) >> MC6470_ACCEL_TAPEN_TAPXPEN_POS)
#define MC6470_ACCEL_TAPEN_TAPXPEN_SET(R_V, F_V)       ((R_V & (~MC6470_ACCEL_TAPEN_TAPXPEN_MASK)) | ((F_V & (MC6470_ACCEL_TAPEN_TAPXPEN_MASK >> MC6470_ACCEL_TAPEN_TAPXPEN_POS)) << MC6470_ACCEL_TAPEN_TAPXPEN_POS))
#define MC6470_ACCEL_TTTRX_TAP_X_QUIET_POS             4
#define MC6470_ACCEL_TTTRX_TAP_X_QUIET_MASK            (0b00001111 << MC6470_ACCEL_TTTRX_TAP_X_QUIET_POS) /**< [7..4] This 4-bit value (0 to 15) sets the number of samples to be ignored after successful tap detection. Detection is rearmed after the specific number of samples has passed. */
#define MC6470_ACCEL_TTTRX_TAP_X_QUIET_GET(R_V)        ((R_V & MC6470_ACCEL_TTTRX_TAP_X_QUIET_MASK) >> MC6470_ACCEL_TTTRX_TAP_X_QUIET_POS)
#define MC6470_ACCEL_TTTRX_TAP_X_QUIET_SET(R_V, F_V)   ((R_V & (~MC6470_ACCEL_TTTRX_TAP_X_QUIET_MASK)) | ((F_V & (MC6470_ACCEL_TTTRX_TAP_X_QUIET_MASK >> MC6470_ACCEL_TTTRX_TAP_X_QUIET_POS)) << MC6470_ACCEL_TTTRX_TAP_X_QUIET_POS))
#define MC6470_ACCEL_TTTRX_TAP_X_DUR_POS               0
#define MC6470_ACCEL_TTTRX_TAP_X_DUR_MASK              (0b00001111 << MC6470_ACCEL_TTTRX_TAP_X_DUR_POS) /**< [3..0] This 4-bit value (0 to 15) sets the maximum number of samples an event must qualify as a tap before it is rejected. For example, if the value is 4, a fast acceleration event which exceeded the threshold for more than 4 consecutive samples would not trigger a tap event. */
#define MC6470_ACCEL_TTTRX_TAP_X_DUR_GET(R_V)          ((R_V & MC6470_ACCEL_TTTRX_TAP_X_DUR_MASK) >> MC6470_ACCEL_TTTRX_TAP_X_DUR_POS)
#define MC6470_ACCEL_TTTRX_TAP_X_DUR_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_TTTRX_TAP_X_DUR_MASK)) | ((F_V & (MC6470_ACCEL_TTTRX_TAP_X_DUR_MASK >> MC6470_ACCEL_TTTRX_TAP_X_DUR_POS)) << MC6470_ACCEL_TTTRX_TAP_X_DUR_POS))
#define MC6470_ACCEL_TTTRY_TAP_Y_QUIET_POS             4
#define MC6470_ACCEL_TTTRY_TAP_Y_QUIET_MASK            (0b00001111 << MC6470_ACCEL_TTTRY_TAP_Y_QUIET_POS) /**< [7..4] This 4-bit value (0 to 15) sets the number of samples to be ignored after successful tap detection. Detection is rearmed after the specific number of samples has passed. */
#define MC6470_ACCEL_TTTRY_TAP_Y_QUIET_GET(R_V)        ((R_V & MC6470_ACCEL_TTTRY_TAP_Y_QUIET_MASK) >> MC6470_ACCEL_TTTRY_TAP_Y_QUIET_POS)
#define MC6470_ACCEL_TTTRY_TAP_Y_QUIET_SET(R_V, F_V)   ((R_V & (~MC6470_ACCEL_TTTRY_TAP_Y_QUIET_MASK)) | ((F_V & (MC6470_ACCEL_TTTRY_TAP_Y_QUIET_MASK >> MC6470_ACCEL_TTTRY_TAP_Y_QUIET_POS)) << MC6470_ACCEL_TTTRY_TAP_Y_QUIET_POS))
#define MC6470_ACCEL_TTTRY_TAP_Y_DUR_POS               0
#define MC6470_ACCEL_TTTRY_TAP_Y_DUR_MASK              (0b00001111 << MC6470_ACCEL_TTTRY_TAP_Y_DUR_POS) /**< [3..0] This 4-bit value (0 to 15) sets the maximum number of samples an event must qualify as a tap before it is rejected. For example, if the value is 4, a fast acceleration event which exceeded the threshold for more than 4 consecutive samples would not trigger a tap event. */
#define MC6470_ACCEL_TTTRY_TAP_Y_DUR_GET(R_V)          ((R_V & MC6470_ACCEL_TTTRY_TAP_Y_DUR_MASK) >> MC6470_ACCEL_TTTRY_TAP_Y_DUR_POS)
#define MC6470_ACCEL_TTTRY_TAP_Y_DUR_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_TTTRY_TAP_Y_DUR_MASK)) | ((F_V & (MC6470_ACCEL_TTTRY_TAP_Y_DUR_MASK >> MC6470_ACCEL_TTTRY_TAP_Y_DUR_POS)) << MC6470_ACCEL_TTTRY_TAP_Y_DUR_POS))
#define MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_POS             4
#define MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_MASK            (0b00001111 << MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_POS) /**< [7..4] This 4-bit value (0 to 15) sets the number of samples to be ignored after successful tap detection. Detection is rearmed after the specific number of samples has passed. */
#define MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_GET(R_V)        ((R_V & MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_MASK) >> MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_POS)
#define MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_SET(R_V, F_V)   ((R_V & (~MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_MASK)) | ((F_V & (MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_MASK >> MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_POS)) << MC6470_ACCEL_TTTRZ_TAP_Z_QUIET_POS))
#define MC6470_ACCEL_TTTRZ_TAP_Z_DUR_POS               0
#define MC6470_ACCEL_TTTRZ_TAP_Z_DUR_MASK              (0b00001111 << MC6470_ACCEL_TTTRZ_TAP_Z_DUR_POS) /**< [3..0] This 4-bit value (0 to 15) sets the maximum number of samples an event must qualify as a tap before it is rejected. For example, if the value is 4, a fast acceleration event which exceeded the threshold for more than 4 consecutive samples would not trigger a tap event. */
#define MC6470_ACCEL_TTTRZ_TAP_Z_DUR_GET(R_V)          ((R_V & MC6470_ACCEL_TTTRZ_TAP_Z_DUR_MASK) >> MC6470_ACCEL_TTTRZ_TAP_Z_DUR_POS)
#define MC6470_ACCEL_TTTRZ_TAP_Z_DUR_SET(R_V, F_V)     ((R_V & (~MC6470_ACCEL_TTTRZ_TAP_Z_DUR_MASK)) | ((F_V & (MC6470_ACCEL_TTTRZ_TAP_Z_DUR_MASK >> MC6470_ACCEL_TTTRZ_TAP_Z_DUR_POS)) << MC6470_ACCEL_TTTRZ_TAP_Z_DUR_POS))
#define MC6470_ACCEL_HWID_HWID_POS                     0
#define MC6470_ACCEL_HWID_HWID_MASK                    (0b11111111 << MC6470_ACCEL_HWID_HWID_POS) /**< [7..0] Hardware ID */
#define MC6470_ACCEL_HWID_HWID_GET(R_V)                ((R_V & MC6470_ACCEL_HWID_HWID_MASK) >> MC6470_ACCEL_HWID_HWID_POS)
#define MC6470_ACCEL_HWID_HWID_SET(R_V, F_V)           ((R_V & (~MC6470_ACCEL_HWID_HWID_MASK)) | ((F_V & (MC6470_ACCEL_HWID_HWID_MASK >> MC6470_ACCEL_HWID_HWID_POS)) << MC6470_ACCEL_HWID_HWID_POS))
#define MC6470_ACCEL_OUTCFG_RANGE_POS                  4
#define MC6470_ACCEL_OUTCFG_RANGE_MASK                 (0b00000111 << MC6470_ACCEL_OUTCFG_RANGE_POS) /**< [6..4] Selects the Accelerometer g Range */
#define MC6470_ACCEL_OUTCFG_RANGE_GET(R_V)             ((R_V & MC6470_ACCEL_OUTCFG_RANGE_MASK) >> MC6470_ACCEL_OUTCFG_RANGE_POS)
#define MC6470_ACCEL_OUTCFG_RANGE_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_OUTCFG_RANGE_MASK)) | ((F_V & (MC6470_ACCEL_OUTCFG_RANGE_MASK >> MC6470_ACCEL_OUTCFG_RANGE_POS)) << MC6470_ACCEL_OUTCFG_RANGE_POS))
#define MC6470_ACCEL_OUTCFG_RES_POS                    0
#define MC6470_ACCEL_OUTCFG_RES_MASK                   (0b00000111 << MC6470_ACCEL_OUTCFG_RES_POS) /**< [2..0] Selects the Accelerometer g Resolution */
#define MC6470_ACCEL_OUTCFG_RES_GET(R_V)               ((R_V & MC6470_ACCEL_OUTCFG_RES_MASK) >> MC6470_ACCEL_OUTCFG_RES_POS)
#define MC6470_ACCEL_OUTCFG_RES_SET(R_V, F_V)          ((R_V & (~MC6470_ACCEL_OUTCFG_RES_MASK)) | ((F_V & (MC6470_ACCEL_OUTCFG_RES_MASK >> MC6470_ACCEL_OUTCFG_RES_POS)) << MC6470_ACCEL_OUTCFG_RES_POS))
#define MC6470_ACCEL_XOFFH_X_GAIN_POS                  1
#define MC6470_ACCEL_XOFFH_X_GAIN_MASK                 (0b00000001 << MC6470_ACCEL_XOFFH_X_GAIN_POS) /**< [1..1] 9th bit of the X Gain register */
#define MC6470_ACCEL_XOFFH_X_GAIN_GET(R_V)             ((R_V & MC6470_ACCEL_XOFFH_X_GAIN_MASK) >> MC6470_ACCEL_XOFFH_X_GAIN_POS)
#define MC6470_ACCEL_XOFFH_X_GAIN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_XOFFH_X_GAIN_MASK)) | ((F_V & (MC6470_ACCEL_XOFFH_X_GAIN_MASK >> MC6470_ACCEL_XOFFH_X_GAIN_POS)) << MC6470_ACCEL_XOFFH_X_GAIN_POS))
#define MC6470_ACCEL_XOFFH_XOFFH_POS                   7
#define MC6470_ACCEL_XOFFH_XOFFH_MASK                  (0b00000001 << MC6470_ACCEL_XOFFH_XOFFH_POS) /**< [7..7] X Offset High */
#define MC6470_ACCEL_XOFFH_XOFFH_GET(R_V)              ((R_V & MC6470_ACCEL_XOFFH_XOFFH_MASK) >> MC6470_ACCEL_XOFFH_XOFFH_POS)
#define MC6470_ACCEL_XOFFH_XOFFH_SET(R_V, F_V)         ((R_V & (~MC6470_ACCEL_XOFFH_XOFFH_MASK)) | ((F_V & (MC6470_ACCEL_XOFFH_XOFFH_MASK >> MC6470_ACCEL_XOFFH_XOFFH_POS)) << MC6470_ACCEL_XOFFH_XOFFH_POS))
#define MC6470_ACCEL_YOFFH_Y_GAIN_POS                  1
#define MC6470_ACCEL_YOFFH_Y_GAIN_MASK                 (0b00000001 << MC6470_ACCEL_YOFFH_Y_GAIN_POS) /**< [1..1] 9th bit of the Y Gain register */
#define MC6470_ACCEL_YOFFH_Y_GAIN_GET(R_V)             ((R_V & MC6470_ACCEL_YOFFH_Y_GAIN_MASK) >> MC6470_ACCEL_YOFFH_Y_GAIN_POS)
#define MC6470_ACCEL_YOFFH_Y_GAIN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_YOFFH_Y_GAIN_MASK)) | ((F_V & (MC6470_ACCEL_YOFFH_Y_GAIN_MASK >> MC6470_ACCEL_YOFFH_Y_GAIN_POS)) << MC6470_ACCEL_YOFFH_Y_GAIN_POS))
#define MC6470_ACCEL_YOFFH_YOFFH_POS                   7
#define MC6470_ACCEL_YOFFH_YOFFH_MASK                  (0b00000001 << MC6470_ACCEL_YOFFH_YOFFH_POS) /**< [7..7] Y Offset High */
#define MC6470_ACCEL_YOFFH_YOFFH_GET(R_V)              ((R_V & MC6470_ACCEL_YOFFH_YOFFH_MASK) >> MC6470_ACCEL_YOFFH_YOFFH_POS)
#define MC6470_ACCEL_YOFFH_YOFFH_SET(R_V, F_V)         ((R_V & (~MC6470_ACCEL_YOFFH_YOFFH_MASK)) | ((F_V & (MC6470_ACCEL_YOFFH_YOFFH_MASK >> MC6470_ACCEL_YOFFH_YOFFH_POS)) << MC6470_ACCEL_YOFFH_YOFFH_POS))
#define MC6470_ACCEL_ZOFFH_Z_GAIN_POS                  1
#define MC6470_ACCEL_ZOFFH_Z_GAIN_MASK                 (0b00000001 << MC6470_ACCEL_ZOFFH_Z_GAIN_POS) /**< [1..1] 9th bit of the Z Gain register */
#define MC6470_ACCEL_ZOFFH_Z_GAIN_GET(R_V)             ((R_V & MC6470_ACCEL_ZOFFH_Z_GAIN_MASK) >> MC6470_ACCEL_ZOFFH_Z_GAIN_POS)
#define MC6470_ACCEL_ZOFFH_Z_GAIN_SET(R_V, F_V)        ((R_V & (~MC6470_ACCEL_ZOFFH_Z_GAIN_MASK)) | ((F_V & (MC6470_ACCEL_ZOFFH_Z_GAIN_MASK >> MC6470_ACCEL_ZOFFH_Z_GAIN_POS)) << MC6470_ACCEL_ZOFFH_Z_GAIN_POS))
#define MC6470_ACCEL_ZOFFH_YOFFH_POS                   7
#define MC6470_ACCEL_ZOFFH_YOFFH_MASK                  (0b00000001 << MC6470_ACCEL_ZOFFH_YOFFH_POS) /**< [7..7] Z Offset High */
#define MC6470_ACCEL_ZOFFH_YOFFH_GET(R_V)              ((R_V & MC6470_ACCEL_ZOFFH_YOFFH_MASK) >> MC6470_ACCEL_ZOFFH_YOFFH_POS)
#define MC6470_ACCEL_ZOFFH_YOFFH_SET(R_V, F_V)         ((R_V & (~MC6470_ACCEL_ZOFFH_YOFFH_MASK)) | ((F_V & (MC6470_ACCEL_ZOFFH_YOFFH_MASK >> MC6470_ACCEL_ZOFFH_YOFFH_POS)) << MC6470_ACCEL_ZOFFH_YOFFH_POS))
#define MC6470_ACCEL_PCODE_PCODE_POS                   0
#define MC6470_ACCEL_PCODE_PCODE_MASK                  (0b11111111 << MC6470_ACCEL_PCODE_PCODE_POS) /**< [7..0] Product Code */
#define MC6470_ACCEL_PCODE_PCODE_GET(R_V)              ((R_V & MC6470_ACCEL_PCODE_PCODE_MASK) >> MC6470_ACCEL_PCODE_PCODE_POS)
#define MC6470_ACCEL_PCODE_PCODE_SET(R_V, F_V)         ((R_V & (~MC6470_ACCEL_PCODE_PCODE_MASK)) | ((F_V & (MC6470_ACCEL_PCODE_PCODE_MASK >> MC6470_ACCEL_PCODE_PCODE_POS)) << MC6470_ACCEL_PCODE_PCODE_POS))

/* This section defines the masks and values for registers that MUST have certain bits set. */
#define MC6470_ACCEL_MODE_ADDR_VALIDATION_VALUE        0b00000000
#define MC6470_ACCEL_MODE_ADDR_VALIDATION_MASK         0b00000100
#define MC6470_ACCEL_HWID_ADDR_VALIDATION_VALUE        0b00000000
#define MC6470_ACCEL_HWID_ADDR_VALIDATION_MASK         0b00000000
#define MC6470_ACCEL_OUTCFG_ADDR_VALIDATION_VALUE      0b00000000
#define MC6470_ACCEL_OUTCFG_ADDR_VALIDATION_MASK       0b10000000



// Sample has been acquired, flag bit is set in polling mode or interrupt mode. 
// This bit cannot be disabled and is always set be hardware when a sample is 
// ready. The host must poll at the sample rate of faster to see this bit 
// transition [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SR_ACQ_INT_NoSampleAvailable      = 0, /**< .  [MC6470_Datasheet] */
	MC6470_ACCEL_SR_ACQ_INT_SampleAvailable        = 1, /**< .  [MC6470_Datasheet] */
} MC6470_ACCEL_SR_ACQ_INT_e;

// One-time Programming (OTP) activity status [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_OPSTAT_OTPA_Idle                  = 0, /**< Internal memory is idle and the device is ready for use.  [MC6470_Datasheet] */
	MC6470_ACCEL_OPSTAT_OTPA_Busy                  = 1, /**< Internal memory is active and the device is not yet ready for use.  [MC6470_Datasheet] */
} MC6470_ACCEL_OPSTAT_OTPA_e;

// I2C watchdog timeout [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_OPSTAT_I2C_WDT_NoEventDetected    = 0, /**< No watchdog event detected.  [MC6470_Datasheet] */
	MC6470_ACCEL_OPSTAT_I2C_WDT_EventDetected      = 1, /**< Watchdog event has been detected by hardware, I2C slave state machine reset to idle. This flag is cleared by reading this register.  [MC6470_Datasheet] */
} MC6470_ACCEL_OPSTAT_I2C_WDT_e;

// Sampling State Register Status, Wait State Register Status [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_OPSTAT_OPSTAT_StandBy             = 0, /**< Device is in STANDBY state, no sampling.  [MC6470_Datasheet] */
	MC6470_ACCEL_OPSTAT_OPSTAT_Wake                = 1, /**< Device is in WAKE state, sampling at set sample rate.  [MC6470_Datasheet] */
} MC6470_ACCEL_OPSTAT_OPSTAT_e;

// Enable Interrupts [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_ACQ_INT_EN_Disable          = 0, /**< Disable automatic interrupt on INTA pad after each sample (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_ACQ_INT_EN_Enable           = 1, /**< Enable automatic interrupt on INTA pad after each sample.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_ACQ_INT_EN_e;

// Negative Z-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIZNEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIZNEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIZNEN_e;

// Positive Z-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIZPEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIZPEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIZPEN_e;

// Negative Y-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIYNEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIYNEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIYNEN_e;

// Positive Y-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIYPEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIYPEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIYPEN_e;

// Negative X-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIZXEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIZXEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIZXEN_e;

// Positive X-axis TAP interrupt enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_INTEN_TIXPEN_Disable              = 0, /**< Disable (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_INTEN_TIXPEN_Enable               = 1, /**< The corresponding TAP enable bit in register 0x09 must be enabled. The INTA pad will transition.  [MC6470_Datasheet] */
} MC6470_ACCEL_INTEN_TIXPEN_e;

// Sets weather the Interrupt pin is Active High or Active Low [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_MODE_IAH_ActiveLow                = 0, /**< Interrupt pin INTA is active low (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_MODE_IAH_ActiveHigh               = 1, /**< Interrupt pin INTA is active high.  [MC6470_Datasheet] */
} MC6470_ACCEL_MODE_IAH_e;

// Sets weather the Interrupt pin Push Pull or Open Drain [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_MODE_IPP_OpenDrain                = 0, /**< Interrupt pin INTA is open drain (default) and requires an external pull-up to VDDIO.  [MC6470_Datasheet] */
	MC6470_ACCEL_MODE_IPP_PushPull                 = 1, /**< Interrupt pin INTA is push-pull. No external pull-up resistor should be installed.  [MC6470_Datasheet] */
} MC6470_ACCEL_MODE_IPP_e;

// Enabled WDT for positive SCL stalls [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_MODE_I2C_WDT_POS_Enabled          = 0, /**< I2C watchdog timer for positive SCL stalls disabled (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_MODE_I2C_WDT_POS_Disabled         = 1, /**< I2C watchdog timer for positive SCL stalls enabled.  [MC6470_Datasheet] */
} MC6470_ACCEL_MODE_I2C_WDT_POS_e;

// Enabled WDT for negative SCL stalls [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_MODE_I2C_WDT_NEG_Enabled          = 0, /**< I2C watchdog timer for negative SCL stalls disabled (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_MODE_I2C_WDT_NEG_Disabled         = 1, /**< I2C watchdog timer for negative SCL stalls enabled.  [MC6470_Datasheet] */
} MC6470_ACCEL_MODE_I2C_WDT_NEG_e;

// Set Device Operational State. WAKE or STANDBY [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_MODE_OPCON_Standby                = 0, /**< Standby State (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_MODE_OPCON_Wake                   = 1, /**< Wake.  [MC6470_Datasheet] */
} MC6470_ACCEL_MODE_OPCON_e;

// Selects weather Multiple Taps or the First Tap is Latched. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SRTFR_TAP_LATCH_MultipleTap       = 0, /**< Multiple TAPs (of those which are enabled) are detected and latched (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_TAP_LATCH_FirstTap          = 1, /**< First TAP detected (e.g. of those enabled) is latched, all others ignored until serviced by reading register 0x03 (Status Register).  [MC6470_Datasheet] */
} MC6470_ACCEL_SRTFR_TAP_LATCH_e;

// Flips the Z Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SRTFR_FLIP_TAPZ_NotFlipped        = 0, /**< Z positive and Z negative tap are not switched (d.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_FLIP_TAPZ_Flipped           = 1, /**< Z positive and Z negative tap are switched.  [MC6470_Datasheet] */
} MC6470_ACCEL_SRTFR_FLIP_TAPZ_e;

// Flips the Y Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SRTFR_FLIP_TAPY_NotFlipped        = 0, /**< Y positive and Y negative tap are not switched (d.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_FLIP_TAPY_Flipped           = 1, /**< Y positive and Y negative tap are switched.  [MC6470_Datasheet] */
} MC6470_ACCEL_SRTFR_FLIP_TAPY_e;

// Flips the X Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SRTFR_FLIP_TAPX_NotFlipped        = 0, /**< X positive and X negative tap are not switched (d.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_FLIP_TAPX_Flipped           = 1, /**< X positive and X negative tap are switched.  [MC6470_Datasheet] */
} MC6470_ACCEL_SRTFR_FLIP_TAPX_e;

// Rate. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_SRTFR_RATE_32HZ                   = 0, /**< 32 Hz (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_16HZ                   = 1, /**< 16 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_8HZ                    = 2, /**< 8 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_4HZ                    = 3, /**< 4 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_2HZ                    = 4, /**< 2 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_1HZ                    = 5, /**< 1 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_0_5HZ                  = 6, /**< 0.5 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_0_25HZ                 = 7, /**< 0.25 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_64HZ                   = 8, /**< 64 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_128HZ                  = 9, /**< 128 Hz.  [MC6470_Datasheet] */
	MC6470_ACCEL_SRTFR_RATE_256HZ                  = 10, /**< 256 Hz.  [MC6470_Datasheet] */
} MC6470_ACCEL_SRTFR_RATE_e;

// Enable Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAP_EN_Disable              = 0, /**< All tap detection is disabled, regardless of bits [5:0] (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAP_EN_Enable               = 1, /**< Tap detection is enabled, individual enables control detection (bits 5-1).  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAP_EN_e;

// Configure weather the TTTRX, TTTRY, and TTTRZ registers hold tap duration and 
// quite period or tap threshold settings. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_THRDUR_DurationAndQuiet     = 0, /**< Registers 0xA (TTTRX), 0xB (TTTRY), 0xC (TTTRZ) point to tap duration and quiet period (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_THRDUR_Threshold            = 1, /**< Registers 0xA (TTTRX), 0xB (TTTRY), 0xC (TTTRZ) point to tap threshold settings.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_THRDUR_e;

// Enable negative Z Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPZNEN_Disable             = 0, /**< Disable negative tap detection on Z-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPZNEN_Enable              = 1, /**< Enable negative tap detection on Z-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPZNEN_e;

// Enable positive Z Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPZPEN_Disable             = 0, /**< Disable positive tap detection on Z-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPZPEN_Enable              = 1, /**< Enable positive tap detection on Z-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPZPEN_e;

// Enable negative Y Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPYNEN_Disable             = 0, /**< Disable negative tap detection on Y-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPYNEN_Enable              = 1, /**< Enable negative tap detection on Y-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPYNEN_e;

// Enable positive Y Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPYPEN_Disable             = 0, /**< Disable positive tap detection on Y-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPYPEN_Enable              = 1, /**< Enable positive tap detection on Y-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPYPEN_e;

// Enable negative X Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPXNEN_Disable             = 0, /**< Disable negative tap detection on X-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPXNEN_Enable              = 1, /**< Enable negative tap detection on X-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPXNEN_e;

// Enable positive X Tap Detection. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_TAPEN_TAPXPEN_Disable             = 0, /**< Disable positive tap detection on X-axis (default).  [MC6470_Datasheet] */
	MC6470_ACCEL_TAPEN_TAPXPEN_Enable              = 1, /**< Enable positive tap detection on X-axis.  [MC6470_Datasheet] */
} MC6470_ACCEL_TAPEN_TAPXPEN_e;

// Hardware ID [mCube_mc6470_mcu_driver] 
typedef enum 
{
	MC6470_ACCEL_HWID_MC3216                       = 35, /**< MC3216.  [mCube_mc6470_mcu_driver] */
} MC6470_ACCEL_HWID_e;

// Selects the Accelerometer g Range [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_OUTCFG_RANGE_2G                   = 0, /**< Select +/- 2g range (Default).  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RANGE_4G                   = 1, /**< Select +/- 4g range.  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RANGE_8G                   = 2, /**< Select +/- 8g range.  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RANGE_16G                  = 3, /**< Select +/- 16g range.  [MC6470_Datasheet] */
} MC6470_ACCEL_OUTCFG_RANGE_e;

// Selects the Accelerometer g Resolution [MC6470_Datasheet] 
typedef enum 
{
	MC6470_ACCEL_OUTCFG_RES_6_Bits                 = 0, /**< Select 6-bits for accelerometer measurements (Default).  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RES_7_Bits                 = 1, /**< Select 7-bit for accelerometer measurements  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RES_8_Bits                 = 2, /**< Select 8-bit for accelerometer measurements.  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RES_10_Bits                = 3, /**< Select 10-bit for accelerometer measurement.  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RES_12_Bits                = 3, /**< Select 12-bit for accelerometer measurements.  [MC6470_Datasheet] */
	MC6470_ACCEL_OUTCFG_RES_14_Bits                = 3, /**< Select 14-bit for accelerometer measurements.  [MC6470_Datasheet] */
} MC6470_ACCEL_OUTCFG_RES_e;

// Product Code [mCube_mc6470_mcu_driver] 
typedef enum 
{
	MC6470_ACCEL_PCODE_3210                        = 144, /**< 3210.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3230                        = 25, /**< 3230.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3250                        = 136, /**< 3250.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3410                        = 171, /**< 3410.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3410N                       = 184, /**< 3410N.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3430                        = 41, /**< 3430.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3430N                       = 57, /**< 3430N.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3510                        = 64, /**< 3510.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3530                        = 48, /**< 3530.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3216                        = 16, /**< 3216.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_3236                        = 96, /**< 3236.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_7030                        = 0, /**< 7030.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_1                  = 32, /**< RESERVED_1.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_2                  = 17, /**< RESERVED_2.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_3                  = 50, /**< RESERVED_3.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_4                  = 97, /**< RESERVED_4.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_5                  = 160, /**< RESERVED_5.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_6                  = 224, /**< RESERVED_6.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_7                  = 145, /**< RESERVED_7.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_8                  = 161, /**< RESERVED_8.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_9                  = 225, /**< RESERVED_8.  [mCube_mc6470_mcu_driver] */
	MC6470_ACCEL_PCODE_RESERVED_10                 = 153, /**< RESERVED_8.  [mCube_mc6470_mcu_driver] */
} MC6470_ACCEL_PCODE_e;



MC6470_reg_value MC6470_ACCEL_validate_register_write(MC6470_reg_addr register_address, MC6470_reg_value value);


#endif
