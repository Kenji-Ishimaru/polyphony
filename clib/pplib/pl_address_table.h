//=======================================================================
// Project Polyphony
//
// File:
//   pl_address_table.h
//
// Abstract:
//   register address table
//
//  Created:
//    6 October 2008
//
// Copyright (c) 2008  Kenji Ishimaru, All rights reserved.
//
//======================================================================
//
// Copyright (c) 2020, Kenji Ishimaru
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//  -Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//  -Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#ifndef __PL_ADDRESS_TABLE_H__
#define __PL_ADDRESS_TABLE_H__

#define DEBUG_PRINTF(...) /*printf(__VA_ARGS__)*/

//#define CACHE_OFF
#define AXI_ACP

//#define USE_SH4_FLOAT_ACCELERATION
//#define USE_SH4_FLOAT_ACCELERATION_LIGHT
//#define USE_HDMI
#define USE_AXI_MONITOR

//#define DEBUG_MS
#define DOUBLE_BUFFER
//#define CSIM_DEBUG
#define PP_CONFIG_MAX_MSTACK 50
#define PP_CONFIG_MAX_LIGHTS 4
#define PP_CONFIG_MAX_MATRIX_PALETTES  16
#define PP_INTERP_BIAS 256.0
#define PP_INTERP_TEX_BIAS 16.0
//#define PP_VU_DRAW_ARRAY_SIZE 513  //should be multiples of 3
#define PP_VU_DRAW_ARRAY_SIZE 10002  //should be multiples of 3
// memory mapping
//   Main Memory is mapped to 0010_0000 to 3fff_ffff, 512MB(0-1fff_ffff)
//   (Accessible to all interconnect masters)
//   Polyphony uses upper-half 256MB 1000_0000-1fff_ffff

// vertex buffer topaddress
#define VTX_TOP0_ADRS 0x11000000
#define VTX_TOP1_ADRS 0x12000000

//#define FLOAT22_INJECTION
#define DUAL_VTX_BUFFER
#define DMA_REG_SET
#define DMA_REG_CMD 0x7F88
// align on sdram bank

#define FB00_ADRS  0x13000000
#define FB01_ADRS  0x14000000
#define FB10_ADRS  0x15000000
#define FB11_ADRS  0x16000000
#define FB20_ADRS  0x17000000
#define FB21_ADRS  0x18000000
#define FB3_ADRS   0x19000000

////////////////// Address Table /////////////////////////////////
// Base Address
#define PP_BASE 0x40000000
#define PP_SYSTEM_BASE PP_BASE
#define PP_RASTER_BASE PP_BASE+0x200
#define PP_VERTEX_BASE PP_BASE+0x300
// System
#define PP_VIDEO_START_DEF    PP_SYSTEM_BASE
#define PP_FB0_OFFSET_DEF     PP_SYSTEM_BASE+0x04
#define PP_FB1_OFFSET_DEF     PP_SYSTEM_BASE+0x08
#define PP_FB0_MS_OFFSET_DEF  PP_SYSTEM_BASE+0x0c
#define PP_FB1_MS_OFFSET_DEF  PP_SYSTEM_BASE+0x10
#define PP_COLOR_MODE_DEF     PP_SYSTEM_BASE+0x14
#define PP_AXI_MASTER_CONFIG_DEF PP_SYSTEM_BASE+0x18
#define PP_STATUS_DEF         PP_SYSTEM_BASE+0x20
#define PP_INT_CLEAR_DEF      PP_SYSTEM_BASE+0x24
#define PP_INT_MASK_DEF       PP_SYSTEM_BASE+0x28
#define PP_FRONT_BUFFER_DEF   PP_SYSTEM_BASE+0x2c
#define PP_DMA_TOP_ADRS0_DEF  PP_SYSTEM_BASE+0x30
#define PP_DMA_TOP_ADRS1_DEF  PP_SYSTEM_BASE+0x34
#define PP_DMA_TOP_ADRS2_DEF  PP_SYSTEM_BASE+0x38
#define PP_DMA_TOP_ADRS3_DEF  PP_SYSTEM_BASE+0x3c
#define PP_DMA_BE_LENGTH_DEF  PP_SYSTEM_BASE+0x40
#define PP_DMA_WD0_DEF        PP_SYSTEM_BASE+0x44
#define PP_DMA_WD1_DEF        PP_SYSTEM_BASE+0x48
#define PP_DMA_CTRL_DEF       PP_SYSTEM_BASE+0x4c
#ifdef USE_AXI_MONITOR
#define PP_AXI_CONF_DEF           PP_SYSTEM_BASE+0x58
#define PP_AXI_MON_80_DEF         PP_SYSTEM_BASE+0x80
#define PP_AXI_MON_84_DEF         PP_SYSTEM_BASE+0x84
#define PP_AXI_MON_88_DEF         PP_SYSTEM_BASE+0x88
#define PP_AXI_MON_8C_DEF         PP_SYSTEM_BASE+0x8c
#define PP_AXI_MON_90_DEF         PP_SYSTEM_BASE+0x90
#define PP_AXI_MON_94_DEF         PP_SYSTEM_BASE+0x94
#define PP_AXI_MON_98_DEF         PP_SYSTEM_BASE+0x98
#define PP_AXI_MON_9C_DEF         PP_SYSTEM_BASE+0x9c
#define PP_AXI_MON_A0_DEF         PP_SYSTEM_BASE+0xa0
#define PP_AXI_MON_A4_DEF         PP_SYSTEM_BASE+0xa4
#define PP_AXI_MON_A8_DEF         PP_SYSTEM_BASE+0xa8
#define PP_AXI_MON_AC_DEF         PP_SYSTEM_BASE+0xac
#define PP_AXI_MON_B0_DEF         PP_SYSTEM_BASE+0xb0
#define PP_AXI_MON_B4_DEF         PP_SYSTEM_BASE+0xb4
#define PP_AXI_MON_B8_DEF         PP_SYSTEM_BASE+0xb8
#define PP_AXI_MON_BC_DEF         PP_SYSTEM_BASE+0xbc
#define PP_AXI_MON_C0_DEF         PP_SYSTEM_BASE+0xc0
#define PP_AXI_MON_C4_DEF         PP_SYSTEM_BASE+0xc4
#define PP_AXI_MON_C8_DEF         PP_SYSTEM_BASE+0xc8
#define PP_AXI_MON_CC_DEF         PP_SYSTEM_BASE+0xcc
#define PP_AXI_MON_D0_DEF         PP_SYSTEM_BASE+0xd0
#define PP_AXI_MON_D4_DEF         PP_SYSTEM_BASE+0xd4
#define PP_AXI_MON_D8_DEF         PP_SYSTEM_BASE+0xd8
#define PP_AXI_MON_DC_DEF         PP_SYSTEM_BASE+0xdc
#define PP_AXI_MON_E0_DEF         PP_SYSTEM_BASE+0xe0
#define PP_AXI_MON_E4_DEF         PP_SYSTEM_BASE+0xe4
#define PP_AXI_MON_E8_DEF         PP_SYSTEM_BASE+0xe8
#define PP_AXI_MON_EC_DEF         PP_SYSTEM_BASE+0xec
#define PP_AXI_MON_F0_DEF         PP_SYSTEM_BASE+0xf0
#define PP_AXI_MON_F4_DEF         PP_SYSTEM_BASE+0xf4
#define PP_AXI_MON_F8_DEF         PP_SYSTEM_BASE+0xf8
#define PP_AXI_MON_FC_DEF         PP_SYSTEM_BASE+0xfc
#endif

// rasterizer registers
#define PP_RASTER_START_DEF      PP_RASTER_BASE
#define PP_RASTER_CACHE_CTRL_DEF PP_RASTER_BASE+0x04
#define PP_VTX_TOP_ADRS_DEF      PP_RASTER_BASE+0x08
#define PP_VTX_TOTAL_SIZE_DEF    PP_RASTER_BASE+0x0c
#define PP_NUM_OF_TRIS_DEF       PP_RASTER_BASE+0x10
#define PP_NUM_OF_ELEMENTS_DEF   PP_RASTER_BASE+0x14
#define PP_VTX_DMA_CTRL_DEF      PP_RASTER_BASE+0x18

#define PP_TEX_OFFSET_DEF        PP_RASTER_BASE+0x20
#define PP_TEX_WIDTH_M1_DEF      PP_RASTER_BASE+0x24
#define PP_TEX_HEIGHT_M1_DEF     PP_RASTER_BASE+0x28
#define PP_TEX_WIDTH_UI_DEF      PP_RASTER_BASE+0x2c
#define PP_TEX_CONFIG_DEF        PP_RASTER_BASE+0x34
#define PP_TEX_CTRL_DEF          PP_RASTER_BASE+0x58
#define PP_TEX_BLEND_CTRL_DEF    PP_RASTER_BASE+0x78

#define PP_SCREEN_MODE_DEF       PP_RASTER_BASE+0x80
#define PP_COLOR_OFFSET_DEF      PP_RASTER_BASE+0x84
#define PP_COLOR_MS_OFFSET_DEF   PP_RASTER_BASE+0x88
#define PP_DEPTH_OFFSET_DEF      PP_RASTER_BASE+0x8c
#define PP_DEPTH_MS_OFFSET_DEF   PP_RASTER_BASE+0x90
#define PP_BLEND_OPERATION_DEF   PP_RASTER_BASE+0x94
#define PP_DEPTH_TEST_DEF        PP_RASTER_BASE+0xac
#define PP_COLOR_MASK_DEF        PP_RASTER_BASE+0xb0

// vertex registers
#define PP_VTX_ATTR_DEF          PP_RASTER_BASE+0xb4
#define PP_VTX0_X_DEF            PP_VERTEX_BASE+0x40
#define PP_VTX0_Y_DEF            PP_VERTEX_BASE+0x44
#define PP_VTX0_Z_DEF            PP_VERTEX_BASE+0x48
#define PP_VTX0_IW_DEF           PP_VERTEX_BASE+0x4c
#define PP_VTX0_P00_DEF          PP_VERTEX_BASE+0x50
#define PP_VTX0_P01_DEF          PP_VERTEX_BASE+0x54
#define PP_VTX0_P02_DEF          PP_VERTEX_BASE+0x58
#define PP_VTX0_P03_DEF          PP_VERTEX_BASE+0x5c
#define PP_VTX0_P10_DEF          PP_VERTEX_BASE+0x60
#define PP_VTX0_P11_DEF          PP_VERTEX_BASE+0x64
#define PP_VTX0_P12_DEF          PP_VERTEX_BASE+0x68
#define PP_VTX0_P13_DEF          PP_VERTEX_BASE+0x6c


#define PP_VTX1_X_DEF            PP_VERTEX_BASE+0x80
#define PP_VTX1_Y_DEF            PP_VERTEX_BASE+0x84
#define PP_VTX1_Z_DEF            PP_VERTEX_BASE+0x88
#define PP_VTX1_IW_DEF           PP_VERTEX_BASE+0x8c
#define PP_VTX1_P00_DEF          PP_VERTEX_BASE+0x90
#define PP_VTX1_P01_DEF          PP_VERTEX_BASE+0x94
#define PP_VTX1_P02_DEF          PP_VERTEX_BASE+0x98
#define PP_VTX1_P03_DEF          PP_VERTEX_BASE+0x9c
#define PP_VTX1_P10_DEF          PP_VERTEX_BASE+0xa0
#define PP_VTX1_P11_DEF          PP_VERTEX_BASE+0xa4
#define PP_VTX1_P12_DEF          PP_VERTEX_BASE+0xa8
#define PP_VTX1_P13_DEF          PP_VERTEX_BASE+0xac

#define PP_VTX2_X_DEF            PP_VERTEX_BASE+0xc0
#define PP_VTX2_Y_DEF            PP_VERTEX_BASE+0xc4
#define PP_VTX2_Z_DEF            PP_VERTEX_BASE+0xc8
#define PP_VTX2_IW_DEF           PP_VERTEX_BASE+0xcc
#define PP_VTX2_P00_DEF          PP_VERTEX_BASE+0xd0
#define PP_VTX2_P01_DEF          PP_VERTEX_BASE+0xd4
#define PP_VTX2_P02_DEF          PP_VERTEX_BASE+0xd8
#define PP_VTX2_P03_DEF          PP_VERTEX_BASE+0xdc
#define PP_VTX2_P10_DEF          PP_VERTEX_BASE+0xe0
#define PP_VTX2_P11_DEF          PP_VERTEX_BASE+0xe4
#define PP_VTX2_P12_DEF          PP_VERTEX_BASE+0xe8
#define PP_VTX2_P13_DEF          PP_VERTEX_BASE+0xec
// System
#define PP_VIDEO_START    (*(volatile unsigned int  *)(PP_VIDEO_START_DEF))
#define PP_FB0_OFFSET     (*(volatile unsigned int  *)(PP_FB0_OFFSET_DEF))
#define PP_FB1_OFFSET     (*(volatile unsigned int  *)(PP_FB1_OFFSET_DEF))
#define PP_FB0_MS_OFFSET  (*(volatile unsigned int  *)(PP_FB0_MS_OFFSET_DEF))
#define PP_FB1_MS_OFFSET  (*(volatile unsigned int  *)(PP_FB1_MS_OFFSET_DEF))
#define PP_COLOR_MODE     (*(volatile unsigned int  *)(PP_COLOR_MODE_DEF))
#define PP_AXI_MASTER_CONFIG  (*(volatile unsigned int  *)(PP_AXI_MASTER_CONFIG_DEF))
#define PP_STATUS         (*(volatile unsigned int  *)(PP_STATUS_DEF))
#define PP_INT_CLEAR      (*(volatile unsigned int  *)(PP_INT_CLEAR_DEF))
#define PP_INT_MASK       (*(volatile unsigned int  *)(PP_INT_MASK_DEF))
#define PP_FRONT_BUFFER   (*(volatile unsigned int  *)(PP_FRONT_BUFFER_DEF))
#define PP_DMA_TOP_ADRS0  (*(volatile unsigned int  *)(PP_DMA_TOP_ADRS0_DEF))
#define PP_DMA_TOP_ADRS1  (*(volatile unsigned int  *)(PP_DMA_TOP_ADRS1_DEF))
#define PP_DMA_TOP_ADRS2  (*(volatile unsigned int  *)(PP_DMA_TOP_ADRS2_DEF))
#define PP_DMA_TOP_ADRS3  (*(volatile unsigned int  *)(PP_DMA_TOP_ADRS3_DEF))
#define PP_DMA_BE_LENGTH  (*(volatile unsigned int  *)(PP_DMA_BE_LENGTH_DEF))
#define PP_DMA_WD0        (*(volatile unsigned int  *)(PP_DMA_WD0_DEF))
#define PP_DMA_WD1        (*(volatile unsigned int  *)(PP_DMA_WD1_DEF))
#define PP_DMA_CTRL       (*(volatile unsigned int  *)(PP_DMA_CTRL_DEF))
#ifdef USE_AXI_MONITOR
#define PP_AXI_CONF       (*(volatile unsigned int  *)(PP_AXI_CONF_DEF))
#define PP_AXI_MON_80     (*(volatile unsigned int  *)(PP_AXI_MON_80_DEF))
#define PP_AXI_MON_84     (*(volatile unsigned int  *)(PP_AXI_MON_84_DEF))
#define PP_AXI_MON_88     (*(volatile unsigned int  *)(PP_AXI_MON_88_DEF))
#define PP_AXI_MON_8C     (*(volatile unsigned int  *)(PP_AXI_MON_8C_DEF))
#define PP_AXI_MON_90     (*(volatile unsigned int  *)(PP_AXI_MON_90_DEF))
#define PP_AXI_MON_94     (*(volatile unsigned int  *)(PP_AXI_MON_94_DEF))
#define PP_AXI_MON_98     (*(volatile unsigned int  *)(PP_AXI_MON_98_DEF))
#define PP_AXI_MON_9C     (*(volatile unsigned int  *)(PP_AXI_MON_9C_DEF))
#define PP_AXI_MON_A0     (*(volatile unsigned int  *)(PP_AXI_MON_A0_DEF))
#define PP_AXI_MON_A4     (*(volatile unsigned int  *)(PP_AXI_MON_A4_DEF))
#define PP_AXI_MON_A8     (*(volatile unsigned int  *)(PP_AXI_MON_A8_DEF))
#define PP_AXI_MON_AC     (*(volatile unsigned int  *)(PP_AXI_MON_AC_DEF))
#define PP_AXI_MON_B0     (*(volatile unsigned int  *)(PP_AXI_MON_B0_DEF))
#define PP_AXI_MON_B4     (*(volatile unsigned int  *)(PP_AXI_MON_B4_DEF))
#define PP_AXI_MON_B8     (*(volatile unsigned int  *)(PP_AXI_MON_B8_DEF))
#define PP_AXI_MON_BC     (*(volatile unsigned int  *)(PP_AXI_MON_BC_DEF))
#define PP_AXI_MON_C0     (*(volatile unsigned int  *)(PP_AXI_MON_C0_DEF))
#define PP_AXI_MON_C4     (*(volatile unsigned int  *)(PP_AXI_MON_C4_DEF))
#define PP_AXI_MON_C8     (*(volatile unsigned int  *)(PP_AXI_MON_C8_DEF))
#define PP_AXI_MON_CC     (*(volatile unsigned int  *)(PP_AXI_MON_CC_DEF))
#define PP_AXI_MON_D0     (*(volatile unsigned int  *)(PP_AXI_MON_D0_DEF))
#define PP_AXI_MON_D4     (*(volatile unsigned int  *)(PP_AXI_MON_D4_DEF))
#define PP_AXI_MON_D8     (*(volatile unsigned int  *)(PP_AXI_MON_D8_DEF))
#define PP_AXI_MON_DC     (*(volatile unsigned int  *)(PP_AXI_MON_DC_DEF))
#define PP_AXI_MON_E0     (*(volatile unsigned int  *)(PP_AXI_MON_E0_DEF))
#define PP_AXI_MON_E4     (*(volatile unsigned int  *)(PP_AXI_MON_E4_DEF))
#define PP_AXI_MON_E8     (*(volatile unsigned int  *)(PP_AXI_MON_E8_DEF))
#define PP_AXI_MON_EC     (*(volatile unsigned int  *)(PP_AXI_MON_EC_DEF))
#define PP_AXI_MON_F0     (*(volatile unsigned int  *)(PP_AXI_MON_F0_DEF))
#define PP_AXI_MON_F4     (*(volatile unsigned int  *)(PP_AXI_MON_F4_DEF))
#define PP_AXI_MON_F8     (*(volatile unsigned int  *)(PP_AXI_MON_F8_DEF))
#define PP_AXI_MON_FC     (*(volatile unsigned int  *)(PP_AXI_MON_FC_DEF))
#endif
// rasterizer registers
#define PP_RASTER_START      (*(volatile unsigned int  *)(PP_RASTER_START_DEF))
#define PP_RASTER_CACHE_CTRL (*(volatile unsigned int  *)(PP_RASTER_CACHE_CTRL_DEF))
#define PP_VTX_TOP_ADRS      (*(volatile unsigned int  *)(PP_VTX_TOP_ADRS_DEF))
#define PP_VTX_TOTAL_SIZE    (*(volatile unsigned int  *)(PP_VTX_TOTAL_SIZE_DEF))
#define PP_NUM_OF_TRIS       (*(volatile unsigned int  *)(PP_NUM_OF_TRIS_DEF))
#define PP_NUM_OF_ELEMENTS   (*(volatile unsigned int  *)(PP_NUM_OF_ELEMENTS_DEF))
#define PP_VTX_DMA_CTRL      (*(volatile unsigned int  *)(PP_VTX_DMA_CTRL_DEF))

#define PP_TEX_OFFSET        (*(volatile unsigned int  *)(PP_TEX_OFFSET_DEF))
#define PP_TEX_WIDTH_M1      (*(volatile unsigned int  *)(PP_TEX_WIDTH_M1_DEF))
#define PP_TEX_HEIGHT_M1     (*(volatile unsigned int  *)(PP_TEX_HEIGHT_M1_DEF))
#define PP_TEX_WIDTH_UI      (*(volatile unsigned int  *)(PP_TEX_WIDTH_UI_DEF))
#define PP_TEX_CONFIG        (*(volatile unsigned int  *)(PP_TEX_CONFIG_DEF))
#define PP_TEX_CTRL          (*(volatile unsigned int  *)(PP_TEX_CTRL_DEF))
#define PP_TEX_BLEND_CTRL    (*(volatile unsigned int  *)(PP_TEX_BLEND_CTRL_DEF))

#define PP_SCREEN_MODE       (*(volatile unsigned int  *)(PP_SCREEN_MODE_DEF))
#define PP_COLOR_OFFSET      (*(volatile unsigned int  *)(PP_COLOR_OFFSET_DEF))
#define PP_COLOR_MS_OFFSET   (*(volatile unsigned int  *)(PP_COLOR_MS_OFFSET_DEF))
#define PP_DEPTH_OFFSET      (*(volatile unsigned int  *)(PP_DEPTH_OFFSET_DEF))
#define PP_DEPTH_MS_OFFSET   (*(volatile unsigned int  *)(PP_DEPTH_MS_OFFSET_DEF))
#define PP_BLEND_OPERATION   (*(volatile unsigned int  *)(PP_BLEND_OPERATION_DEF))
#define PP_DEPTH_TEST        (*(volatile unsigned int  *)(PP_DEPTH_TEST_DEF))
#define PP_COLOR_MASK        (*(volatile unsigned int  *)(PP_COLOR_MASK_DEF))

// vertex registers
#define PP_VTX_ATTR (*(volatile unsigned int  *)(PP_VTX_ATTR_DEF))
#define PP_VTX0_X   (*(volatile unsigned int  *)(PP_VTX0_X_DEF))
#define PP_VTX0_Y   (*(volatile unsigned int  *)(PP_VTX0_Y_DEF))
#define PP_VTX0_Z   (*(volatile unsigned int  *)(PP_VTX0_Z_DEF))
#define PP_VTX0_IW  (*(volatile unsigned int  *)(PP_VTX0_IW_DEF))
#define PP_VTX0_P00 (*(volatile unsigned int  *)(PP_VTX0_P00_DEF))
#define PP_VTX0_P01 (*(volatile unsigned int  *)(PP_VTX0_P01_DEF))
#define PP_VTX0_P02 (*(volatile unsigned int  *)(PP_VTX0_P02_DEF))
#define PP_VTX0_P03 (*(volatile unsigned int  *)(PP_VTX0_P03_DEF))
#define PP_VTX0_P10 (*(volatile unsigned int  *)(PP_VTX0_P10_DEF))
#define PP_VTX0_P11 (*(volatile unsigned int  *)(PP_VTX0_P11_DEF))
#define PP_VTX0_P12 (*(volatile unsigned int  *)(PP_VTX0_P12_DEF))
#define PP_VTX0_P13 (*(volatile unsigned int  *)(PP_VTX0_P13_DEF))

#define PP_VTX1_X   (*(volatile unsigned int  *)(PP_VTX1_X_DEF))
#define PP_VTX1_Y   (*(volatile unsigned int  *)(PP_VTX1_Y_DEF))
#define PP_VTX1_Z   (*(volatile unsigned int  *)(PP_VTX1_Z_DEF))
#define PP_VTX1_IW  (*(volatile unsigned int  *)(PP_VTX1_IW_DEF))
#define PP_VTX1_P00 (*(volatile unsigned int  *)(PP_VTX1_P00_DEF))
#define PP_VTX1_P01 (*(volatile unsigned int  *)(PP_VTX1_P01_DEF))
#define PP_VTX1_P02 (*(volatile unsigned int  *)(PP_VTX1_P02_DEF))
#define PP_VTX1_P03 (*(volatile unsigned int  *)(PP_VTX1_P03_DEF))
#define PP_VTX1_P10 (*(volatile unsigned int  *)(PP_VTX1_P10_DEF))
#define PP_VTX1_P11 (*(volatile unsigned int  *)(PP_VTX1_P11_DEF))
#define PP_VTX1_P12 (*(volatile unsigned int  *)(PP_VTX1_P12_DEF))
#define PP_VTX1_P13 (*(volatile unsigned int  *)(PP_VTX1_P13_DEF))

#define PP_VTX2_X   (*(volatile unsigned int  *)(PP_VTX2_X_DEF))
#define PP_VTX2_Y   (*(volatile unsigned int  *)(PP_VTX2_Y_DEF))
#define PP_VTX2_Z   (*(volatile unsigned int  *)(PP_VTX2_Z_DEF))
#define PP_VTX2_IW  (*(volatile unsigned int  *)(PP_VTX2_IW_DEF))
#define PP_VTX2_P00 (*(volatile unsigned int  *)(PP_VTX2_P00_DEF))
#define PP_VTX2_P01 (*(volatile unsigned int  *)(PP_VTX2_P01_DEF))
#define PP_VTX2_P02 (*(volatile unsigned int  *)(PP_VTX2_P02_DEF))
#define PP_VTX2_P03 (*(volatile unsigned int  *)(PP_VTX2_P03_DEF))
#define PP_VTX2_P10 (*(volatile unsigned int  *)(PP_VTX2_P10_DEF))
#define PP_VTX2_P11 (*(volatile unsigned int  *)(PP_VTX2_P11_DEF))
#define PP_VTX2_P12 (*(volatile unsigned int  *)(PP_VTX2_P12_DEF))
#define PP_VTX2_P13 (*(volatile unsigned int  *)(PP_VTX2_P13_DEF))

#ifdef USE_HDMI
#define PP_I2C_PRER_LO (*(volatile unsigned int  *)(PP_BASE+0x100))
#define PP_I2C_PRER_HI (*(volatile unsigned int  *)(PP_BASE+0x104))
#define PP_I2C_CTR     (*(volatile unsigned int  *)(PP_BASE+0x108))
// write
#define PP_I2C_TXR     (*(volatile unsigned int  *)(PP_BASE+0x10c))
// read
#define PP_I2C_RXR     (*(volatile unsigned int  *)(PP_BASE+0x10c))
// write
#define PP_I2C_CR      (*(volatile unsigned int  *)(PP_BASE+0x110))
// read
#define PP_I2C_SR      (*(volatile unsigned int  *)(PP_BASE+0x110))
#endif
// Vertex Unit
#define PP_VU_TOP              0x1000
#define PP_SET_VIEWPORT        0x1000
#define PP_SET_VIEW_MATRIX     0x1001
#define PP_SET_MODEL_MATRIX    0x1002
#define PP_SET_PROJECTION_MATRIX     0x1003
#define PP_SET_VERTEX4         0x1004
#define PP_SET_COLOR4          0x1005
#define PP_SET_TEXCOORD2       0x1006
#define PP_SET_NORMAL3         0x1007


// Rasterize Unit
#define PP_RU_TOP              0x2000
#define PP_SET_TRIANGLE        0x2000

// Pixel Unit
#define PP_PU_TOP              0x3000


// Control Unit
#define PP_CU_TOP                    0x4000
#define PP_SET_SCREEN_WIDTH          0x4000
#define PP_SET_SCREEN_HEIGHT         0x4001
#define PP_SET_FRAME_BUFFER0_OFFSET  0x4010
#define PP_SET_FRAME_BUFFER1_OFFSET  0x4011
#define PP_SET_DEPTH_BUFFER_OFFSET   0x4012
#define PP_SET_TEXTURE_BUFFER_OFFSET 0x4013
#define PP_SET_VERTEX_BUFFER_OFFSET  0x4014

#define PP_CLEAR_FRAME_BUFFER0       0x4020
#define PP_CLEAR_FRAME_BUFFER1       0x4021
#define PP_CLEAR_DEPTH_BUFFER        0x4022
#define PP_TRANS_TEXTURE             0x4023

#define PP_SET_CULLFACE_ENABLE       0x4030

// Texture                         
#define PP_SET_TEXTURE_ENABLE        0x4080
#define PP_SET_TEXTURE_WIDTH         0x4081
#define PP_SET_TEXTURE_HEIGHT        0x4082
// Lighting
#define PP_SET_LIGHTING_ENABLE       0x4100
#define PP_SET_LIGHT0_ENABLE         0x4101
// Materials
#define PP_SET_MATERIAL_AMBIENT_F    0x4200
#define PP_SET_MATERIAL_DIFFUSE_F    0x4201
#define PP_SET_MATERIAL_SPECULAR_F   0x4202
#define PP_SET_MATERIAL_EMISSION_F   0x4203
#define PP_SET_MATERIAL_SHININESS_F  0x4204
#define PP_SET_MATERIAL_AMBIENT_B    0x4205
#define PP_SET_MATERIAL_DIFFUSE_B    0x4206
#define PP_SET_MATERIAL_SPECULAR_B   0x4207
#define PP_SET_MATERIAL_EMISSION_B   0x4208
#define PP_SET_MATERIAL_SHININESS_B  0x4209
// Light0
#define PP_SET_LIGHT0_AMBIENT        0x4300
#define PP_SET_LIGHT0_DIFFUSE        0x4301
#define PP_SET_LIGHT0_SPECULAR       0x4302
#define PP_SET_LIGHT0_POSITION       0x4303
#define PP_SET_LIGHT0_SPOTDIR        0x4304
#define PP_SET_LIGHT0_SPOTEXP        0x4305
#define PP_SET_LIGHT0_SPOTCUTOFF     0x4306
#define PP_SET_LIGHT0_CONST_ATT      0x4307
#define PP_SET_LIGHT0_LINEAR_ATT     0x4308
#define PP_SET_LIGHT0_QUAD_ATT       0x4309

// Local Memory
#define PP_LOCAL_MEMORY_TOP          0x10000


///////////////////////////////////////////////////////////////////

//////////////// Data Bus /////////////////////////////////////////

typedef struct  {
    float r, g, b;
} svf_color3;

typedef struct {
    float r, g, b, a;
} svf_color4;

typedef struct  {
    float u, v;
} svf_texture2;

typedef struct {
    float x, y, z, w;
} svf_position4;

typedef struct {
    float x, y, z;
} svf_position3;

// triangle data structure from pp_vu to pp_ru
typedef struct  {
    int wx, wy;
    union {
        struct {
            float fx;
            float fy;
            float fz;
            float fw;
        } element;
        float f[4];
    } coord;
    float fiw;
    union {
        struct {  // to eye coordinate
            float fx;
            float fy;
            float fz;
            float fw;
        } element;
        float f[4];
    } coord_eye;
    union {
        struct {  // normal
            float fx;
            float fy;
            float fz;
            float fw;
        } element;
        float f[4];
    } n;
    svf_color4    c;
    svf_texture2  t;
} svf_vertex;



typedef struct  {
    svf_vertex v[3];
} svf_triangle;

//////////////////////////////////////////////////////////////////////////

#endif
