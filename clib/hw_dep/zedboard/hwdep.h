//=======================================================================
// Project Polyphony
//
// File:
//   hwdep.h
//
// Abstract:
//   CPU dependent routines header file
//
//  Created:
//    2 February 2009
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

#ifndef __HWDEP_H__
#define __HWDEP_H__

#include "platform.h"
#include "xstatus.h"
#include "xil_types.h"
#include "xil_assert.h"
#include "xuartps_hw.h"
#include "xparameters_ps.h"

#include "pplib/pplib.h"
#include "pplib/pl_address_table.h"
#include "inthandler.h"

float get_timer_count(int ns_count);
int timer_init();
void timer_start();
void timer_end(int kind);
void wait_vsync();
void int_config();
void system_init();
void pci_init();
void rasterizer_init();
void show_render_report();

extern void init_status(struct render_status *s);
extern void copy_status(struct render_status *d, struct render_status *s);
#ifdef USE_HDMI
void i2c_init();
void i2c_write(int sadrs,int adrs,int wdata);
int i2c_read(int sadrs,int adrs);
void i2c_write_conf(int sadrs,int adrs,int wdata,int exp);

#endif

//#define CACHE_OFF
#define AXI_ACP

#ifdef USE_AXI_MONITOR
void show_axi_monitor();
#endif

#endif
