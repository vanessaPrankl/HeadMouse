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

#ifndef __MC6470_MAG_DEFS_H__
#define __MC6470_MAG_DEFS_H__

#include "mc6470_defs.h"

#define MC6470_MAG_ADDRESS_7_BITS                      0x0C /**<  */

#define MC6470_MAG_SELF_TEST_ADDR                      0x0C /**< Self Test Response  [MC6470_Datasheet] */
#define MC6470_MAG_MORE_INFO_VERSION_ADDR              0x0D /**< Information Value 1.  [MC6470_Datasheet] */
#define MC6470_MAG_MORE_INFO_ADDR                      0x0E /**< Information Value 2.  [MC6470_Datasheet] */
#define MC6470_MAG_WHO_AM_I_ADDR                       0x0F /**< Who am I.  [MC6470_Datasheet] */
#define MC6470_MAG_X_AXIS_LSB_ADDR                     0x10 /**< X-Axis least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_X_AXIS_MSB_ADDR                     0x11 /**< X-Axis most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Y_AXIS_LSB_ADDR                     0x12 /**< Y-Axis least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Y_AXIS_MSB_ADDR                     0x13 /**< Y-Axis most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Z_AXIS_LSB_ADDR                     0x14 /**< Z-Axis least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Z_AXIS_MSB_ADDR                     0x15 /**< Z-Axis most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_STATUS_ADDR                         0x18 /**< Status.  [MC6470_Datasheet] */
#define MC6470_MAG_CTRL_1_ADDR                         0x1B /**< Control 1.  [MC6470_Datasheet] */
#define MC6470_MAG_CTRL_2_ADDR                         0x1C /**< Control 2 Register  [MC6470_Datasheet] */
#define MC6470_MAG_CTRL_3_ADDR                         0x1D /**< Control 3 Register  [MC6470_Datasheet] */
#define MC6470_MAG_CTRL_4_ADDR                         0x1E /**< Control 4 Register  [MC6470_Datasheet] */
#define MC6470_MAG_X_AXIS_OFFSET_LSB_ADDR              0x20 /**< X-Axis offset drift least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_X_AXIS_OFFSET_MSB_ADDR              0x21 /**< X-Axis offset drift most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Y_AXIS_OFFSET_LSB_ADDR              0x22 /**< Y-Axis offset drift least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Y_AXIS_OFFSET_MSB_ADDR              0x23 /**< Y-Axis offset drift most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Z_AXIS_OFFSET_LSB_ADDR              0x24 /**< Z-Axis offset drift least significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_Z_AXIS_OFFSET_MSB_ADDR              0x25 /**< Z-Axis offset drift most significant byte.  [MC6470_Datasheet] */
#define MC6470_MAG_ITHR_LSB_ADDR                       0x26 /**< ITHR_LSB  [MC6470_Datasheet] */
#define MC6470_MAG_ITHR_MSB_ADDR                       0x26 /**< ITHR_MSB  [MC6470_Datasheet] */
#define MC6470_MAG_TEMPERATURE_ADDR                    0x31 /**< The result of a temperature reading after CTRL3.TCS is set to 1. This value is used to perform temperature compensation of the sensor values.  [MC6470_Datasheet] */

#define MC6470_MAG_STATUS_DRDY_POS                     6
#define MC6470_MAG_STATUS_DRDY_MASK                    (0b00000001 << MC6470_MAG_STATUS_DRDY_POS) /**< [6..6] Data Ready Detection */
#define MC6470_MAG_STATUS_DRDY_GET(R_V)                ((R_V & MC6470_MAG_STATUS_DRDY_MASK) >> MC6470_MAG_STATUS_DRDY_POS)
#define MC6470_MAG_STATUS_DRDY_SET(R_V, F_V)           ((R_V & (~MC6470_MAG_STATUS_DRDY_MASK)) | ((F_V & (MC6470_MAG_STATUS_DRDY_MASK >> MC6470_MAG_STATUS_DRDY_POS)) << MC6470_MAG_STATUS_DRDY_POS))
#define MC6470_MAG_STATUS_DOR_POS                      5
#define MC6470_MAG_STATUS_DOR_MASK                     (0b00000001 << MC6470_MAG_STATUS_DOR_POS) /**< [5..5] Data Overrun Detection */
#define MC6470_MAG_STATUS_DOR_GET(R_V)                 ((R_V & MC6470_MAG_STATUS_DOR_MASK) >> MC6470_MAG_STATUS_DOR_POS)
#define MC6470_MAG_STATUS_DOR_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_STATUS_DOR_MASK)) | ((F_V & (MC6470_MAG_STATUS_DOR_MASK >> MC6470_MAG_STATUS_DOR_POS)) << MC6470_MAG_STATUS_DOR_POS))
#define MC6470_MAG_STATUS_FFU_POS                      2
#define MC6470_MAG_STATUS_FFU_MASK                     (0b00000001 << MC6470_MAG_STATUS_FFU_POS) /**< [2..2] FFU */
#define MC6470_MAG_STATUS_FFU_GET(R_V)                 ((R_V & MC6470_MAG_STATUS_FFU_MASK) >> MC6470_MAG_STATUS_FFU_POS)
#define MC6470_MAG_STATUS_FFU_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_STATUS_FFU_MASK)) | ((F_V & (MC6470_MAG_STATUS_FFU_MASK >> MC6470_MAG_STATUS_FFU_POS)) << MC6470_MAG_STATUS_FFU_POS))
#define MC6470_MAG_STATUS_TRDY_POS                     1
#define MC6470_MAG_STATUS_TRDY_MASK                    (0b00000001 << MC6470_MAG_STATUS_TRDY_POS) /**< [1..1] TRDY */
#define MC6470_MAG_STATUS_TRDY_GET(R_V)                ((R_V & MC6470_MAG_STATUS_TRDY_MASK) >> MC6470_MAG_STATUS_TRDY_POS)
#define MC6470_MAG_STATUS_TRDY_SET(R_V, F_V)           ((R_V & (~MC6470_MAG_STATUS_TRDY_MASK)) | ((F_V & (MC6470_MAG_STATUS_TRDY_MASK >> MC6470_MAG_STATUS_TRDY_POS)) << MC6470_MAG_STATUS_TRDY_POS))
#define MC6470_MAG_STATUS_ORDY_POS                     0
#define MC6470_MAG_STATUS_ORDY_MASK                    (0b00000001 << MC6470_MAG_STATUS_ORDY_POS) /**< [0..0] ORDY */
#define MC6470_MAG_STATUS_ORDY_GET(R_V)                ((R_V & MC6470_MAG_STATUS_ORDY_MASK) >> MC6470_MAG_STATUS_ORDY_POS)
#define MC6470_MAG_STATUS_ORDY_SET(R_V, F_V)           ((R_V & (~MC6470_MAG_STATUS_ORDY_MASK)) | ((F_V & (MC6470_MAG_STATUS_ORDY_MASK >> MC6470_MAG_STATUS_ORDY_POS)) << MC6470_MAG_STATUS_ORDY_POS))
#define MC6470_MAG_CTRL_1_PC_POS                       7
#define MC6470_MAG_CTRL_1_PC_MASK                      (0b00000001 << MC6470_MAG_CTRL_1_PC_POS) /**< [7..7] Power Mode Control */
#define MC6470_MAG_CTRL_1_PC_GET(R_V)                  ((R_V & MC6470_MAG_CTRL_1_PC_MASK) >> MC6470_MAG_CTRL_1_PC_POS)
#define MC6470_MAG_CTRL_1_PC_SET(R_V, F_V)             ((R_V & (~MC6470_MAG_CTRL_1_PC_MASK)) | ((F_V & (MC6470_MAG_CTRL_1_PC_MASK >> MC6470_MAG_CTRL_1_PC_POS)) << MC6470_MAG_CTRL_1_PC_POS))
#define MC6470_MAG_CTRL_1_ODR_POS                      3
#define MC6470_MAG_CTRL_1_ODR_MASK                     (0b00000011 << MC6470_MAG_CTRL_1_ODR_POS) /**< [4..3] Output Data Rate Control in Normal State */
#define MC6470_MAG_CTRL_1_ODR_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_1_ODR_MASK) >> MC6470_MAG_CTRL_1_ODR_POS)
#define MC6470_MAG_CTRL_1_ODR_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_1_ODR_MASK)) | ((F_V & (MC6470_MAG_CTRL_1_ODR_MASK >> MC6470_MAG_CTRL_1_ODR_POS)) << MC6470_MAG_CTRL_1_ODR_POS))
#define MC6470_MAG_CTRL_1_FS_POS                       1
#define MC6470_MAG_CTRL_1_FS_MASK                      (0b00000001 << MC6470_MAG_CTRL_1_FS_POS) /**< [1..1] State control in Active Mode */
#define MC6470_MAG_CTRL_1_FS_GET(R_V)                  ((R_V & MC6470_MAG_CTRL_1_FS_MASK) >> MC6470_MAG_CTRL_1_FS_POS)
#define MC6470_MAG_CTRL_1_FS_SET(R_V, F_V)             ((R_V & (~MC6470_MAG_CTRL_1_FS_MASK)) | ((F_V & (MC6470_MAG_CTRL_1_FS_MASK >> MC6470_MAG_CTRL_1_FS_POS)) << MC6470_MAG_CTRL_1_FS_POS))
#define MC6470_MAG_CTRL_2_AVG_POS                      7
#define MC6470_MAG_CTRL_2_AVG_MASK                     (0b00000001 << MC6470_MAG_CTRL_2_AVG_POS) /**< [7..7] Average */
#define MC6470_MAG_CTRL_2_AVG_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_2_AVG_MASK) >> MC6470_MAG_CTRL_2_AVG_POS)
#define MC6470_MAG_CTRL_2_AVG_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_2_AVG_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_AVG_MASK >> MC6470_MAG_CTRL_2_AVG_POS)) << MC6470_MAG_CTRL_2_AVG_POS))
#define MC6470_MAG_CTRL_2_FCO_POS                      6
#define MC6470_MAG_CTRL_2_FCO_MASK                     (0b00000001 << MC6470_MAG_CTRL_2_FCO_POS) /**< [6..6] FCO */
#define MC6470_MAG_CTRL_2_FCO_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_2_FCO_MASK) >> MC6470_MAG_CTRL_2_FCO_POS)
#define MC6470_MAG_CTRL_2_FCO_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_2_FCO_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_FCO_MASK >> MC6470_MAG_CTRL_2_FCO_POS)) << MC6470_MAG_CTRL_2_FCO_POS))
#define MC6470_MAG_CTRL_2_AOR_POS                      5
#define MC6470_MAG_CTRL_2_AOR_MASK                     (0b00000001 << MC6470_MAG_CTRL_2_AOR_POS) /**< [5..5] AOR */
#define MC6470_MAG_CTRL_2_AOR_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_2_AOR_MASK) >> MC6470_MAG_CTRL_2_AOR_POS)
#define MC6470_MAG_CTRL_2_AOR_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_2_AOR_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_AOR_MASK >> MC6470_MAG_CTRL_2_AOR_POS)) << MC6470_MAG_CTRL_2_AOR_POS))
#define MC6470_MAG_CTRL_2_FF_POS                       4
#define MC6470_MAG_CTRL_2_FF_MASK                      (0b00000001 << MC6470_MAG_CTRL_2_FF_POS) /**< [4..4] FF */
#define MC6470_MAG_CTRL_2_FF_GET(R_V)                  ((R_V & MC6470_MAG_CTRL_2_FF_MASK) >> MC6470_MAG_CTRL_2_FF_POS)
#define MC6470_MAG_CTRL_2_FF_SET(R_V, F_V)             ((R_V & (~MC6470_MAG_CTRL_2_FF_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_FF_MASK >> MC6470_MAG_CTRL_2_FF_POS)) << MC6470_MAG_CTRL_2_FF_POS))
#define MC6470_MAG_CTRL_2_DTS_POS                      1
#define MC6470_MAG_CTRL_2_DTS_MASK                     (0b00000001 << MC6470_MAG_CTRL_2_DTS_POS) /**< [1..1] DTS */
#define MC6470_MAG_CTRL_2_DTS_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_2_DTS_MASK) >> MC6470_MAG_CTRL_2_DTS_POS)
#define MC6470_MAG_CTRL_2_DTS_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_2_DTS_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_DTS_MASK >> MC6470_MAG_CTRL_2_DTS_POS)) << MC6470_MAG_CTRL_2_DTS_POS))
#define MC6470_MAG_CTRL_2_DOS_POS                      0
#define MC6470_MAG_CTRL_2_DOS_MASK                     (0b00000001 << MC6470_MAG_CTRL_2_DOS_POS) /**< [0..0] DOS */
#define MC6470_MAG_CTRL_2_DOS_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_2_DOS_MASK) >> MC6470_MAG_CTRL_2_DOS_POS)
#define MC6470_MAG_CTRL_2_DOS_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_2_DOS_MASK)) | ((F_V & (MC6470_MAG_CTRL_2_DOS_MASK >> MC6470_MAG_CTRL_2_DOS_POS)) << MC6470_MAG_CTRL_2_DOS_POS))
#define MC6470_MAG_CTRL_3_SRST_POS                     7
#define MC6470_MAG_CTRL_3_SRST_MASK                    (0b00000001 << MC6470_MAG_CTRL_3_SRST_POS) /**< [7..7] Soft Reset Control Enable */
#define MC6470_MAG_CTRL_3_SRST_GET(R_V)                ((R_V & MC6470_MAG_CTRL_3_SRST_MASK) >> MC6470_MAG_CTRL_3_SRST_POS)
#define MC6470_MAG_CTRL_3_SRST_SET(R_V, F_V)           ((R_V & (~MC6470_MAG_CTRL_3_SRST_MASK)) | ((F_V & (MC6470_MAG_CTRL_3_SRST_MASK >> MC6470_MAG_CTRL_3_SRST_POS)) << MC6470_MAG_CTRL_3_SRST_POS))
#define MC6470_MAG_CTRL_3_FRC_POS                      6
#define MC6470_MAG_CTRL_3_FRC_MASK                     (0b00000001 << MC6470_MAG_CTRL_3_FRC_POS) /**< [6..6] Start to measure in force state */
#define MC6470_MAG_CTRL_3_FRC_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_3_FRC_MASK) >> MC6470_MAG_CTRL_3_FRC_POS)
#define MC6470_MAG_CTRL_3_FRC_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_3_FRC_MASK)) | ((F_V & (MC6470_MAG_CTRL_3_FRC_MASK >> MC6470_MAG_CTRL_3_FRC_POS)) << MC6470_MAG_CTRL_3_FRC_POS))
#define MC6470_MAG_CTRL_3_STC_POS                      4
#define MC6470_MAG_CTRL_3_STC_MASK                     (0b00000001 << MC6470_MAG_CTRL_3_STC_POS) /**< [4..4] Self Test Control Enable */
#define MC6470_MAG_CTRL_3_STC_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_3_STC_MASK) >> MC6470_MAG_CTRL_3_STC_POS)
#define MC6470_MAG_CTRL_3_STC_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_3_STC_MASK)) | ((F_V & (MC6470_MAG_CTRL_3_STC_MASK >> MC6470_MAG_CTRL_3_STC_POS)) << MC6470_MAG_CTRL_3_STC_POS))
#define MC6470_MAG_CTRL_3_TCS_POS                      1
#define MC6470_MAG_CTRL_3_TCS_MASK                     (0b00000001 << MC6470_MAG_CTRL_3_TCS_POS) /**< [1..1] Start to measure temperature in active mode. */
#define MC6470_MAG_CTRL_3_TCS_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_3_TCS_MASK) >> MC6470_MAG_CTRL_3_TCS_POS)
#define MC6470_MAG_CTRL_3_TCS_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_3_TCS_MASK)) | ((F_V & (MC6470_MAG_CTRL_3_TCS_MASK >> MC6470_MAG_CTRL_3_TCS_POS)) << MC6470_MAG_CTRL_3_TCS_POS))
#define MC6470_MAG_CTRL_3_OCL_POS                      0
#define MC6470_MAG_CTRL_3_OCL_MASK                     (0b00000001 << MC6470_MAG_CTRL_3_OCL_POS) /**< [0..0] Start to measure temperature in active mode. */
#define MC6470_MAG_CTRL_3_OCL_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_3_OCL_MASK) >> MC6470_MAG_CTRL_3_OCL_POS)
#define MC6470_MAG_CTRL_3_OCL_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_3_OCL_MASK)) | ((F_V & (MC6470_MAG_CTRL_3_OCL_MASK >> MC6470_MAG_CTRL_3_OCL_POS)) << MC6470_MAG_CTRL_3_OCL_POS))
#define MC6470_MAG_CTRL_4_MMD_POS                      6
#define MC6470_MAG_CTRL_4_MMD_MASK                     (0b00000011 << MC6470_MAG_CTRL_4_MMD_POS) /**< [7..6] MMD */
#define MC6470_MAG_CTRL_4_MMD_GET(R_V)                 ((R_V & MC6470_MAG_CTRL_4_MMD_MASK) >> MC6470_MAG_CTRL_4_MMD_POS)
#define MC6470_MAG_CTRL_4_MMD_SET(R_V, F_V)            ((R_V & (~MC6470_MAG_CTRL_4_MMD_MASK)) | ((F_V & (MC6470_MAG_CTRL_4_MMD_MASK >> MC6470_MAG_CTRL_4_MMD_POS)) << MC6470_MAG_CTRL_4_MMD_POS))
#define MC6470_MAG_CTRL_4_RS_POS                       4
#define MC6470_MAG_CTRL_4_RS_MASK                      (0b00000001 << MC6470_MAG_CTRL_4_RS_POS) /**< [4..4] Set the dynamic range of output data */
#define MC6470_MAG_CTRL_4_RS_GET(R_V)                  ((R_V & MC6470_MAG_CTRL_4_RS_MASK) >> MC6470_MAG_CTRL_4_RS_POS)
#define MC6470_MAG_CTRL_4_RS_SET(R_V, F_V)             ((R_V & (~MC6470_MAG_CTRL_4_RS_MASK)) | ((F_V & (MC6470_MAG_CTRL_4_RS_MASK >> MC6470_MAG_CTRL_4_RS_POS)) << MC6470_MAG_CTRL_4_RS_POS))
#define MC6470_MAG_CTRL_4_AS_POS                       3
#define MC6470_MAG_CTRL_4_AS_MASK                      (0b00000001 << MC6470_MAG_CTRL_4_AS_POS) /**< [3..3] AS */
#define MC6470_MAG_CTRL_4_AS_GET(R_V)                  ((R_V & MC6470_MAG_CTRL_4_AS_MASK) >> MC6470_MAG_CTRL_4_AS_POS)
#define MC6470_MAG_CTRL_4_AS_SET(R_V, F_V)             ((R_V & (~MC6470_MAG_CTRL_4_AS_MASK)) | ((F_V & (MC6470_MAG_CTRL_4_AS_MASK >> MC6470_MAG_CTRL_4_AS_POS)) << MC6470_MAG_CTRL_4_AS_POS))
#define MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_POS           0
#define MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_MASK          (0b00111111 << MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_POS) /**< [5..0] MSB */
#define MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_GET(R_V)      ((R_V & MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_MASK) >> MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_POS)
#define MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_SET(R_V, F_V) ((R_V & (~MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_MASK)) | ((F_V & (MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_MASK >> MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_POS)) << MC6470_MAG_X_AXIS_OFFSET_MSB_MSB_POS))
#define MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_POS           0
#define MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_MASK          (0b00111111 << MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_POS) /**< [5..0] MSB */
#define MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_GET(R_V)      ((R_V & MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_MASK) >> MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_POS)
#define MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_SET(R_V, F_V) ((R_V & (~MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_MASK)) | ((F_V & (MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_MASK >> MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_POS)) << MC6470_MAG_Y_AXIS_OFFSET_MSB_MSB_POS))
#define MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_POS           0
#define MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_MASK          (0b00111111 << MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_POS) /**< [5..0] MSB */
#define MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_GET(R_V)      ((R_V & MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_MASK) >> MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_POS)
#define MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_SET(R_V, F_V) ((R_V & (~MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_MASK)) | ((F_V & (MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_MASK >> MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_POS)) << MC6470_MAG_Z_AXIS_OFFSET_MSB_MSB_POS))
#define MC6470_MAG_ITHR_MSB_MSB_POS                    0
#define MC6470_MAG_ITHR_MSB_MSB_MASK                   (0b00111111 << MC6470_MAG_ITHR_MSB_MSB_POS) /**< [5..0] MSB */
#define MC6470_MAG_ITHR_MSB_MSB_GET(R_V)               ((R_V & MC6470_MAG_ITHR_MSB_MSB_MASK) >> MC6470_MAG_ITHR_MSB_MSB_POS)
#define MC6470_MAG_ITHR_MSB_MSB_SET(R_V, F_V)          ((R_V & (~MC6470_MAG_ITHR_MSB_MSB_MASK)) | ((F_V & (MC6470_MAG_ITHR_MSB_MSB_MASK >> MC6470_MAG_ITHR_MSB_MSB_POS)) << MC6470_MAG_ITHR_MSB_MSB_POS))

/* This section defines the masks and values for registers that MUST have certain bits set. */
#define MC6470_MAG_STATUS_ADDR_VALIDATION_VALUE        0b00000000
#define MC6470_MAG_STATUS_ADDR_VALIDATION_MASK         0b00000111
#define MC6470_MAG_CTRL_2_ADDR_VALIDATION_VALUE        0b00000000
#define MC6470_MAG_CTRL_2_ADDR_VALIDATION_MASK         0b11110011



// Data Ready Detection [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_STATUS_DRDY_DataReady               = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_STATUS_DRDY_DataNotReady            = 1, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_STATUS_DRDY_e;

// Data Overrun Detection [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_STATUS_DOR_NoDataOverrun            = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_STATUS_DOR_DataOverrun              = 1, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_STATUS_DOR_e;

// Power Mode Control [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_1_PC_StandByMode               = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_1_PC_ActiveMode                = 1, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_1_PC_e;

// Output Data Rate Control in Normal State [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_1_ODR_0_5HZ                    = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_1_ODR_10HZ                     = 1, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_1_ODR_20HZ                     = 2, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_1_ODR_100HZ                    = 3, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_1_ODR_e;

// State control in Active Mode [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_1_FS_Normal                    = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_1_FS_Force                     = 1, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_1_FS_e;

// Average [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_AVG_Default                  = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_AVG_e;

// FCO [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_FCO_Default                  = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_FCO_e;

// AOR [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_AOR_Default                  = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_AOR_e;

// FF [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_FF_Default                   = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_FF_e;

// DTS [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_DTS_Default                  = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_DTS_e;

// DOS [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_2_DOS_Default                  = 0, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_2_DOS_e;

// Soft Reset Control Enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_3_SRST_Default                 = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_3_SRST_Reset                   = 1, /**< Soft Reset.  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_3_SRST_e;

// Start to measure in force state [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_3_FRC_Default                  = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_3_FRC_Start                    = 1, /**< Measurement Start.  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_3_FRC_e;

// Self Test Control Enable [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_3_STC_Default                  = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_3_STC_Start                    = 1, /**< Set parameters to Self Test Response (STB) register.  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_3_STC_e;

// Start to measure temperature in active mode. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_3_TCS_Default                  = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_3_TCS_Start                    = 1, /**< Start Temperature measurements  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_3_TCS_e;

// Start to measure temperature in active mode. [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_3_OCL_Default                  = 0, /**< .  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_3_OCL_Start                    = 1, /**< Start offset calibration  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_3_OCL_e;

// MMD [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_4_MMD_Default                  = 2, /**< .  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_4_MMD_e;

// Set the dynamic range of output data [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_4_RS_14_BIT                    = 0, /**< 14 Bit signed value (-8192 to 8191).  [MC6470_Datasheet] */
	MC6470_MAG_CTRL_4_RS_15_BIT                    = 1, /**< 15 Bit signed value (-16384 to 16383).  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_4_RS_e;

// AS [MC6470_Datasheet] 
typedef enum 
{
	MC6470_MAG_CTRL_4_AS_Default                   = 0, /**< Field must be set to 0.  [MC6470_Datasheet] */
} MC6470_MAG_CTRL_4_AS_e;



MC6470_reg_value MC6470_MAG_validate_register_write(MC6470_reg_addr register_address, MC6470_reg_value value);


#endif
