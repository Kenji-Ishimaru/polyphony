//=======================================================================
//                        Project Polyphony
//
// File:
//   hwdep.c
//
// Abstract:
//   CPU dependent routines
//
//  Created:
//    2 February 2009
//
// Copyright (c) 2008  Kenji Ishimaru, All rights reserved.
//
//======================================================================
//  Revision History
#include <stdio.h>
#include "hwdep.h"
#include "pplib/pl_vu.h"
/***************************** Include Files *********************************/
// for scutimer
#include "xparameters.h"
#include "xscutimer.h"
#include "xil_printf.h"

/************************** Constant Definitions *****************************/

volatile int network_detected;

// Global registers for analysys
extern struct render_status rstat_tmp; 
extern struct render_status rstat_out; 

XScuTimer Timer;		/* Cortex A9 SCU Private Timer Instance */

#define TIMER_DEVICE_ID		XPAR_XSCUTIMER_0_DEVICE_ID
#define TIMER_LOAD_VALUE	0x7FFFFFFF

int timer_init() {
/*
	int Status;
	volatile u32 CntValue1 = 0;
	volatile u32 CntValue2 = 0;
	XScuTimer_Config *ConfigPtr;
	XScuTimer *TimerInstancePtr = &Timer;

	ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

	Status = XScuTimer_CfgInitialize(TimerInstancePtr, ConfigPtr,
				 ConfigPtr->BaseAddr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return XST_SUCCESS;
*/
}


float get_timer_count(int ns_count) {
//    return ns_count/30720; // 30ns x 1024 (PCK/1024)
    return ns_count*3.0/1000000000.0; // 1cycle = 333MHz, return sec
}

void timer_start() {
/*
	XScuTimer *TimerInstancePtr = &Timer;
	if (!timer_working) {
	  XScuTimer_LoadTimer(TimerInstancePtr, TIMER_LOAD_VALUE);
	  prev_val = XScuTimer_GetCounterValue(TimerInstancePtr);
	  XScuTimer_Start(TimerInstancePtr);
	  timer_working = 1;
	}
*/
}

void timer_end(int kind) {
/*
	float f;
	XScuTimer *TimerInstancePtr = &Timer;
	cur_val = XScuTimer_GetCounterValue(TimerInstancePtr);
	//printf("prev,cur %x %x %d\n",prev_val,cur_val,prev_val-cur_val);
	f = get_timer_count(prev_val-cur_val);
	if (kind == 0) {
        //printf("geo sec. = %f\n", f);
        rstat_tmp.geometry_processing_time += f;
    } else if (kind == 1) {
        //printf("ras sec. = %f\n", f);
        rstat_tmp.rasterize_processing_time += f;
    } else 
        printf("dma sec. = %f\n", f);
	timer_working = 0;
*/
}

void wait_vsync() {
#ifdef DOUBLE_BUFFER
#ifdef DUAL_VTX_BUFFER
   if (!first_vsync) {
        while (!render_dma_end);
        render_end = 1;
   }
#else
   render_end = 1;
#endif
   int_vsync = 0;
   while (!int_vsync);
#ifndef DUAL_VTX_BUFFER
   timer_end(1);
#else
   timer_end(0);  // geo
   timer_end(1);  // ras
   timer_start();
#endif

   rstat_tmp.num_of_frames++;
   // copy status
   copy_status(&rstat_out, &rstat_tmp);

   rstat_tmp.num_of_injected_triangles = 0;
   rstat_tmp.num_of_visible_triangles = 0;
   rstat_tmp.geometry_processing_time = 0;
   rstat_tmp.rasterize_processing_time = 0;

   render_end = 0;
#endif
#ifdef DUAL_VTX_BUFFER
   // start rasterize(buffer clear -> rasterize -> cache flush)
   // buffer swap
   // register configuration
   first_vsync = 0;
   cache_init();
   buffer_clear_setting();
   next_render_setting();
   swap_vtx_buffer();
#ifndef CACHE_OFF
#ifndef AXI_ACP
   Xil_DCacheFlush();
#endif
#endif
   vu_clear_start();
#endif

}

void int_config() {
  int xstatus;
  xstatus = ScuGicInterrupt_Init();
	if (xstatus != XST_SUCCESS) {
		printf("Interrupt initialization failed\n");
		while(1);
	}
}

void rasterizer_init() {
#ifdef DOUBLE_BUFFER
#ifdef DEBUG_MS
  PP_FB0_OFFSET    = FB01_ADRS;
  PP_FB0_MS_OFFSET = FB01_ADRS;
#else
  PP_FB0_OFFSET    = FB00_ADRS;
  PP_FB0_MS_OFFSET = FB01_ADRS;
#endif

#else
  PP_FB0_OFFSET    = FB10_ADRS;
  PP_FB0_MS_OFFSET = FB11_ADRS;
#endif

#ifdef DEBUG_MS
  PP_FB1_OFFSET    = FB11_ADRS;
  PP_FB1_MS_OFFSET = FB11_ADRS;
#else
  PP_FB1_OFFSET    = FB10_ADRS;
  PP_FB1_MS_OFFSET = FB11_ADRS;
#endif

  PP_COLOR_OFFSET    = FB10_ADRS;
  PP_COLOR_MS_OFFSET = FB11_ADRS;
  PP_DEPTH_OFFSET    = FB20_ADRS;
  PP_DEPTH_MS_OFFSET = FB21_ADRS;

  // Color mode 5:6:5
  PP_COLOR_MODE = 0x00000000;
  PP_FRONT_BUFFER = 0;
  PP_INT_MASK = 0;

}


void system_init() {
	int i;
    init_platform();

#ifdef CACHE_OFF
Xil_DCacheDisable();
//Xil_ICacheDisable();
#endif
  int_config();
  // Timer init
  i = timer_init();
  if (i != 0) {
    printf("timer init failed\n");
    while(1);
  }
  // global registers
  render_end = 0;
  int_vsync = 0;
#ifdef USE_HDMI
  i2c_init();

  // Power-up the Tx(HPD must be high)
   i2c_write_conf(0x39,0x41,0x10,0x10);
   // Fixed registers that must be set on power up
   i2c_write_conf(0x39,0x98,0x03,0x3);
   i2c_write_conf(0x39,0x9a,0xe0,0xe0);
   i2c_write_conf(0x39,0x9c,0x30,0x30);
   i2c_write(0x39,0x9d,0x61);
   //i2c_write_conf(0x39,0x9d,0x61,0x61);
   i2c_write_conf(0x39,0xa2,0xa4,0xa4);
   i2c_write_conf(0x39,0xa3,0xa4,0xa4);
   i2c_write_conf(0x39,0xe0,0xd0,0xd0);
   i2c_write_conf(0x39,0x55,0x12,0x12);
   i2c_write_conf(0x39,0xf9,0x00,0x00);
   // Input mode
   i2c_write_conf(0x39,0x15,0x01,0x01);  // Video format: YCbCr 4:2:2 separate sync
   i2c_write_conf(0x39,0x48,0x08,0x08);  // Right justified
   i2c_write_conf(0x39,0x16,0x38,0x38);  // Input Style = 1, Color Depth= 8, Output 4:4:4
   i2c_write_conf(0x39,0x17,0x00,0x00);  // Aspect ratio 4:3
   // Output mode
   i2c_write_conf(0x39,0xaf,0x00,0x00);  // DVI mode
   i2c_write_conf(0x39,0x4c,0x04,0x04);
   i2c_write_conf(0x39,0x40,0x00,0x00);

   // -----------------------------------------------
   // -- YCrCb => RGB conversion
   // -- HDTV YCbCr (16 to 255) to RGB (0 to 255)
   // -----------------------------------------------
  i2c_write(0x39,0x18,0xe7);
  i2c_write(0x39,0x19,0x34);
  i2c_write(0x39,0x1a,0x04);
  i2c_write(0x39,0x1b,0xad);
  i2c_write(0x39,0x1c,0x00);
  i2c_write(0x39,0x1d,0x00);
  i2c_write(0x39,0x1e,0x1c);
  i2c_write(0x39,0x1f,0x1b);

  i2c_write(0x39,0x20,0x1d);
  i2c_write(0x39,0x21,0xdc);
  i2c_write(0x39,0x22,0x04);
  i2c_write(0x39,0x23,0xad);
  i2c_write(0x39,0x24,0x1f);
  i2c_write(0x39,0x25,0x24);
  i2c_write(0x39,0x26,0x01);
  i2c_write(0x39,0x27,0x35);

  i2c_write(0x39,0x28,0x00);
  i2c_write(0x39,0x29,0x00);
  i2c_write(0x39,0x2a,0x04);
  i2c_write(0x39,0x2b,0xad);
  i2c_write(0x39,0x2c,0x08);
  i2c_write(0x39,0x2d,0x7c);
  i2c_write(0x39,0x2e,0x1b);
  i2c_write(0x39,0x2f,0x77);
#endif
  // analysis registers
  init_status(&rstat_tmp);
  init_status(&rstat_out);

  printf("Process Start\n");

  // Hardware initialization
  printf("Rasterizer initialization\n");
  rasterizer_init();
  // AXI master configuration
  PP_AXI_MASTER_CONFIG = 0xffffffff;
  // Video Start
  printf("Video Start\n");
  PP_VIDEO_START = 0x00000003;  // stop for debugging
}

#ifdef USE_HDMI
void i2c_init() {
    PP_I2C_PRER_LO = 0x0b;
    PP_I2C_PRER_HI = 0x00;
    PP_I2C_CTR  = 0x80;  // enable
}

static int sleep() {
    int i,x = 0;
    for (i=0;i<10000000;i++)
        x = PP_I2C_SR;
    return x;
}

void i2c_write_conf(int sadrs,int adrs,int wdata,int exp) {
    int rd = -1;
    int i = 0;
    int max_cnt = 100;
    while (rd != exp) {
        i2c_write(sadrs,adrs,wdata);
        rd = i2c_read(sadrs,adrs);
        if (i > max_cnt) {
            printf("i2c write error a:%x r:%x e:%x\n",adrs,rd,exp);
            break;
        }
        i++;
    }
}

void i2c_write(int sadrs,int adrs,int wdata) {
    int rdata;

    PP_I2C_TXR = sadrs << 1;  // Tx: Slave Address
    PP_I2C_CR = 0x91;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
        rdata = PP_I2C_SR ;  // Command
    PP_I2C_TXR =  adrs;  // Tx: Memory Address
    PP_I2C_CR = 0x11;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
        rdata = PP_I2C_SR ;  // Command
    PP_I2C_TXR = wdata;  // Tx: Write Data
    PP_I2C_CR = 0x51;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
        rdata = PP_I2C_SR ;  // Command
}

int i2c_read(int sadrs,int adrs) {
    int rdata;
    rdata = PP_I2C_SR ;  // Command
    // set address(write)
    PP_I2C_TXR = sadrs<<1;  // Tx: Slave Address
    PP_I2C_CR = 0x91;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
    	rdata = PP_I2C_SR ;  // Command
    PP_I2C_TXR =  adrs;  // Tx: Memory Address
    PP_I2C_CR = 0x11;  // Command (DO NOT SEND STOP BIT)
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
      rdata = PP_I2C_SR ;  // Command
    // read data
    PP_I2C_TXR = sadrs<<1 | 0x1;  // Tx: Slave Address
    PP_I2C_CR = 0x91;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
        rdata = PP_I2C_SR ;  // Command
    PP_I2C_CR = 0x61;  // Command
    rdata = PP_I2C_SR ;  // Command
    while (!(rdata & 1))
        rdata = PP_I2C_SR ;  // Command
    return PP_I2C_RXR;
}

#endif

void show_render_report() {
    // stop chip scope
    *((unsigned int *)(PP_SYSTEM_BASE+0x58)) = 1;
    printf("********************* render report *******************\n");
    // accumulation
    printf("total_injected_triangles %d\n",rstat_out.total_injected_triangles);
    printf("total_visible_triangles %d\n",rstat_out.total_visible_triangles);
    printf("total_geometry_processing_time %f\n",rstat_out.total_geometry_processing_time);
    printf("total_rasterize_processing_time %f\n",rstat_out.total_rasterize_processing_time);
    // calc average
    printf("average_injected_triangles %d\n",rstat_out.average_injected_triangles);
    printf("average_visible_triangles %d\n",rstat_out.average_visible_triangles);
    printf("average_geometry_processing_time %f\n",rstat_out.average_geometry_processing_time);
    printf("average_rasterize_processing_time %f\n",rstat_out.average_rasterize_processing_time);
#ifdef USE_AXI_MONITOR
    show_axi_monitor();
#endif
}
#ifdef USE_AXI_MONITOR
void show_axi_monitor() {
}
#endif
