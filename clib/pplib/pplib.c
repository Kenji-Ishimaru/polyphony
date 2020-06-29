//=======================================================================
// Project Polyphony
//
// File:
//   pplib.c
//
// Abstract:
//   Graphics library implementation
//
//  Created:
//    22 July 2008
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

#include "pplib.h"
#include "pl_vu.h"
#include "pl_vertex4.h"
#include "pl_vector4.h"
#include "pl_matrix3.h"
#include "pl_matrix4.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

// Global registers for analysys
struct render_status rstat_tmp; 
struct render_status rstat_out; 

extern pl_vu_state vu_state;
extern float fTBL[4096];

// static objects

// global registers
volatile int render_end;
volatile int int_vsync;
volatile int dma_end;
volatile int render_dma_end;
volatile int psoc_on;
volatile int psoc_off;
volatile int psoc_stat_prev;

int render_data_available;
int cur_vtx_gen_bank;
int cur_vtx_ras_bank;
int cur_color_bank_assign;
int first_vsync;
unsigned int attr_pointer;

// vertex buffer infomation
t_vtx_buffer s_vtx_buffer[2];

pl_matrix4                    cur_pm;    // current projection matrix
pl_matrix4                    cur_tm;    // current texture matrix
pl_matrix4                    cur_mm;    // current model matrix
pl_matrix4                    cur_vm;    // current view matrix
pl_matrix4                    cur_cm;    // current color matrix

pl_matrix4*                   cur_matrix;  // current matrix stack
pl_matrix4                    matrix_stack[PP_CONFIG_MAX_MSTACK];
int                           matrix_stack_ptr;

// states
bool b_gl_texture_2d;
bool b_gl_lighting;
bool b_gl_light[PP_CONFIG_MAX_LIGHTS];
bool has_color;
bool has_tex;
bool has_normal;
bool has_weight;
bool has_matrix_index;
bool b_gl_multisample;
bool b_gl_color_clear;
bool b_gl_depth_clear;
bool b_buffer_blend;
// current state
float current_t[2];  // u, v
float current_c[4];  // r, g, b, a
float current_n[3];  // nx, ny nz

// clear
float clear_color[4];
float clear_z;
// depth function
e_depth_mode m_depth_mode;
bool b_gl_depth_test;


// texture environment
unsigned int m_cur_gen_tex;
unsigned int m_bind_tex;

s_texture_environment s_texenv[PP_MAX_TEXENV];

// conversion function
unsigned int wb[4];


t_fui u_fui;

void ftoui(int n, float bi[]) {
    int i;
    for (i = 0; i < n; i++) {
        u_fui.f = bi[i];
        wb[i] = u_fui.ui;
    }
    if (n <4) {
        u_fui.f = 1.0;
        wb[i] = u_fui.ui;
    }
}

float tsin(float x) {
    float r, result;
    float mm_2pi, mm_pi_1z, mm_pi_2z, mm_pi_3z, tbl_steps;
    float tmp;
    int idx;
    r = x;
    if (r < 0) r = -r;
    mm_2pi = MM_PI;
    tmp = 2.0;
    mm_2pi *= tmp;
    mm_pi_1z = MM_PI;
    mm_pi_1z /= tmp;
    mm_pi_2z = MM_PI;
    mm_pi_3z = mm_pi_1z + mm_pi_2z;
    tbl_steps = TBL_STEPS;
    while (r >= mm_2pi) {
      r -= mm_2pi;
    }
    if (x < 0) r = mm_2pi - r;
    if (r < mm_pi_1z) {
        idx = (int)( r/mm_pi_1z * tbl_steps);
        result = fTBL[idx];
    } else if (r < mm_pi_2z) {
        r = r - mm_pi_1z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        idx = ~idx & (TBL_STEPS-1);
        result = fTBL[idx];
    } else if (r < mm_pi_3z) {
      r = r - mm_pi_2z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        result = fTBL[idx];
        result = -result;
    } else {
        r = r - mm_pi_3z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        idx = ~idx & (TBL_STEPS-1);
        result = fTBL[idx];
        result = -result;
    }
    return result;
}

float tcos(float x) {
    float r, result;
    float mm_2pi, mm_pi_1z, mm_pi_2z, mm_pi_3z, tbl_steps;
    float tmp;
    int idx;
    r = x;
    if (r < 0) r = -r;
    //scif_puts ("in tsin pass0\r\n");
    mm_2pi = MM_PI;
    tmp = 2.0;
    mm_2pi *= tmp;
    mm_pi_1z = MM_PI;
    mm_pi_1z /= tmp;
    mm_pi_2z = MM_PI;
    mm_pi_3z = mm_pi_1z + mm_pi_2z;
    tbl_steps = TBL_STEPS;
    while (r >= mm_2pi) {
      r -= mm_2pi;
    }
    //scif_puts ("in tsin pass1\r\n");
    if (x < 0) r = mm_2pi - r;
    if (r < mm_pi_1z) {
        r = r - mm_pi_1z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        idx = ~idx & (TBL_STEPS-1);
        result = fTBL[idx];
    } else if (r < mm_pi_2z) {
      r = r - mm_pi_2z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        result = fTBL[idx];
        result = -result;
    } else if (r < mm_pi_3z) {
        r = r - mm_pi_3z;
        idx = (int)( r/mm_pi_1z * tbl_steps);
        idx = ~idx & (TBL_STEPS-1);
        result = fTBL[idx];
        result = -result;
    } else {
        idx = (int)( r/mm_pi_1z * tbl_steps);
        result = fTBL[idx];
    }
    return result;
}

void gl_init() {
    int i,j;
    pl_matrix4_identity(&cur_pm);
    pl_matrix4_identity(&cur_tm);
    pl_matrix4_identity(&cur_mm);
    pl_matrix4_identity(&cur_vm);
    pl_matrix4_identity(&cur_cm);
    matrix_stack_ptr = 0;
    cur_matrix = &cur_mm;  // initial matrix mode is model-view

    unsigned int d;
    d = 640;
    vu_state.m_vp_width = d;
    d = 480;
    vu_state.m_vp_height = d;

    // states
    b_gl_texture_2d = false;
    b_gl_lighting = false;
    has_color = false;
    has_tex = false;
    has_normal = false;
    has_weight = false;
    has_matrix_index = false;
    b_gl_multisample = false;
    b_gl_color_clear = false;
    b_gl_depth_clear = false;

    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++)
        b_gl_light[i] = false;

    // current state
    for (i = 0; i<2; i++)
        current_t[i] = 0;
    for (i = 0; i<4; i++)
        current_c[i] = 0;
    for (i = 0; i<3; i++)
        current_n[i] = 0;
    // depth function
    b_gl_depth_test = false;
    m_depth_mode = LESS;
    // texture environment
    m_cur_gen_tex = 0;
    m_bind_tex = 0;

    for (i = 0; i < PP_MAX_TEXENV;i++) {
        s_texenv[i].tex_address = FB3_ADRS;  // initial texture offset
        s_texenv[i].m_tex_format = R8G8B8A8; 
        // texture blender
        for (j = 0; j<3; j++) {
            s_texenv[i].m_tb_arg[0][j] = TB_ARG_PRIMARY_COLOR;
        }
    }

    // buffer control
    s_vtx_buffer[0].top_address = VTX_TOP0_ADRS;
    s_vtx_buffer[0].pci_top_address = VTX_TOP0_ADRS;
    s_vtx_buffer[0].total_size = 0;
    s_vtx_buffer[0].num_of_triangles = 0;
    s_vtx_buffer[1].top_address = VTX_TOP1_ADRS;
    s_vtx_buffer[1].pci_top_address = VTX_TOP1_ADRS;
    s_vtx_buffer[1].total_size = 0;
    s_vtx_buffer[1].num_of_triangles = 0;
    render_dma_end = 0;
    render_data_available = 0;
    cur_vtx_gen_bank = 0;
    cur_vtx_ras_bank = 0;
    b_buffer_blend = false;
    // 
    cur_color_bank_assign = 0;
    first_vsync = 1;
    // vertex processor state init
    vu_init();
}

void color_buffer_all_clear() {
	vu_clear_color_buffer_all();
}


void ct_set_gold() {
	vu_ct_set(0);
}

void ct_set_silver(){
	vu_ct_set(1);
}


void backdoor_color_config(int c) {
//11 = 8:8:8:8
//10 = 4:4:4:4
//01 = 5:5:5:1
//00 = 5:6:5

    // Color mode 5:6:5
    PP_COLOR_MODE = c;
}

void set_all_matrix() {
    vu_set_current_matrix0(PL_PROJECTION_MATRIX, cur_pm.a);
    vu_set_current_matrix0(PL_MODEL_MATRIX, cur_mm.a);
    vu_set_current_matrix0(PL_VIEW_MATRIX, cur_vm.a);

    vu_matrix_prep();
}

void render_buffered_triangles() {
    render_dma_start();
    wait_render_end();
}


void set_tex_enable(bool b) {
    set_3d_reg(PP_TEX_CTRL_DEF,b);
}

void set_depth_enable(int func, bool b) {
    unsigned int d;
    d = func << 16;
    d |= b;
    set_3d_reg(PP_DEPTH_TEST_DEF,d);
}

void wait_render_end() {
    // wait render dma
	printf("waiting for render dma end\n");
    while (!render_dma_end) ;
    // clear data
    s_vtx_buffer[cur_vtx_ras_bank].total_size = 0;
    s_vtx_buffer[cur_vtx_ras_bank].num_of_triangles = 0;
}

void cache_flush() {
    unsigned int x;
    int i;
    // wait a wile before cache flush
    for (i = 0; i<1000; i++)
        x = PP_RASTER_CACHE_CTRL;
    // cache flush
    PP_RASTER_CACHE_CTRL = 0x100;
    x = PP_RASTER_CACHE_CTRL;
    x &= 0x100;
    while (x == 0x100) {
        x = PP_RASTER_CACHE_CTRL;
        x &= 0x100;
    }
}

void cache_init() {
    PP_RASTER_CACHE_CTRL = 1;
}

void render_dma_start() {
    unsigned int ua;
    unsigned int i;
    printf("render dma start\n");
    while (!dma_end) ; // wait for buffer clear
    printf("render dma end\n");
    timer_end(0);
    if (render_triangle_available()) {
        render_dma_end = 0;
        ua = 0;
        if (has_color) {
            ua = 0x00003001;
        }    
        if (has_tex) {
            ua |= 0x12010000;
        }
        if (has_color & has_tex) vu_texture_blender_en(true);  // currently only support multiply color & tex
        else vu_texture_blender_en(false);


        PP_VTX_ATTR = ua;

        PP_VTX_TOP_ADRS = s_vtx_buffer[cur_vtx_gen_bank].pci_top_address;
        PP_VTX_TOTAL_SIZE = s_vtx_buffer[cur_vtx_gen_bank].total_size;
        PP_NUM_OF_TRIS = s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles;
        rstat_tmp.num_of_visible_triangles += s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles;
        printf("adrs %x, size %d, num_tri %d\n",
        		 s_vtx_buffer[cur_vtx_gen_bank].pci_top_address,
        		 s_vtx_buffer[cur_vtx_gen_bank].total_size,
        		 s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles
        		 );
        for (i=0;i<s_vtx_buffer[cur_vtx_gen_bank].total_size;i++)
        	printf("a : %x d: %x\n",i*4+s_vtx_buffer[cur_vtx_gen_bank].pci_top_address, *((volatile unsigned int *)(i*4+s_vtx_buffer[cur_vtx_gen_bank].pci_top_address)));
        PP_VTX_DMA_CTRL = 1; // render start
        timer_start();
    } else render_dma_end = 1;
}

void swap_vtx_buffer() {
    // swap buffer pointers
    if (cur_vtx_gen_bank ==0) {
        cur_vtx_gen_bank = 1;
        cur_vtx_ras_bank = 0;
    } else {
        cur_vtx_gen_bank = 0;
        cur_vtx_ras_bank = 1;
    }
    // clear next buffer 
    s_vtx_buffer[cur_vtx_gen_bank].total_size = 0;
    s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles = 0;
}

void next_render_setting() {
    if (render_triangle_available()) {
        render_dma_end = 0;

        PP_VTX_TOP_ADRS = s_vtx_buffer[cur_vtx_gen_bank].pci_top_address;
        PP_VTX_TOTAL_SIZE = s_vtx_buffer[cur_vtx_gen_bank].total_size;
        PP_NUM_OF_TRIS = s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles;
        rstat_tmp.num_of_visible_triangles += s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles;
    } else {
        render_dma_end = 1;
    }
}

void buffer_clear_setting() {
    if (b_gl_color_clear) {
        vu_clear_color_buffer(false, clear_color);
        if (b_gl_multisample|b_buffer_blend)
            vu_clear_color_buffer(true, clear_color);
    }
    if (b_gl_depth_clear) {
        vu_clear_depth_buffer(false, clear_z);
        if (b_gl_multisample|b_buffer_blend)
            vu_clear_depth_buffer(true, clear_z);
    }
}

bool render_triangle_available() {
    if (s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles > 0) return true;
    else return false;
}

void set_3d_reg(unsigned int adrs, unsigned int data) {
#ifdef DMA_REG_SET
    unsigned int cur_base, cur_ptr,t;
    
    cur_base = s_vtx_buffer[cur_vtx_gen_bank].top_address;
    cur_ptr =  s_vtx_buffer[cur_vtx_gen_bank].total_size;

    // set address
    t = adrs & 0xff;
    t |= (DMA_REG_CMD << 16);
    (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = t;
    cur_ptr++;
    // set data
    (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = data;
    cur_ptr++;
    // set new pointer
    s_vtx_buffer[cur_vtx_gen_bank].total_size = cur_ptr;
#else
    (*(volatile unsigned int  *)adrs) = data;
#endif
}

unsigned int get_current_buffer_ptr() {
    unsigned int cur_ptr;
    cur_ptr =  s_vtx_buffer[cur_vtx_gen_bank].total_size;
    return cur_ptr -2;  // rewind adrs and wdata
}

void set_3d_reg_ptr(unsigned int adrs, unsigned int data, unsigned int ptr) {
    unsigned int cur_base, cur_ptr,t;
    
    cur_base = s_vtx_buffer[cur_vtx_gen_bank].top_address;
    cur_ptr =  ptr;

    // set address
    t = adrs & 0xff;
    t |= (DMA_REG_CMD << 16);
    (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = t;
    cur_ptr++;
    // set data
    (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = data;
}


void set_buffer_blend(bool en) {
    // VGA output reads 2 color buffers and blends for anaglyph
    unsigned int x;
    b_buffer_blend = true;
    x = PP_VIDEO_START;
    if (en) x |= 0x10300;
    else x &= 0xff;
    PP_VIDEO_START = x;
}

void copy_status(struct render_status *d, struct render_status *s) {
    d->num_of_frames = s->num_of_frames;
    d->num_of_injected_triangles = s->num_of_injected_triangles;
    d->num_of_visible_triangles = s->num_of_visible_triangles;
    d->render_fps = s->render_fps;
    d->geometry_processing_time = s->geometry_processing_time;
    d->rasterize_processing_time = s->rasterize_processing_time;

    // accumulation
    d->total_injected_triangles += s->num_of_injected_triangles;
    d->total_visible_triangles += s->num_of_visible_triangles;
    d->total_geometry_processing_time += s->geometry_processing_time;
    d->total_rasterize_processing_time += s->rasterize_processing_time;
    // calc average
    d->average_injected_triangles = d->total_injected_triangles/d->num_of_frames;
    d->average_visible_triangles = d->total_visible_triangles/d->num_of_frames;
    d->average_geometry_processing_time = d->total_geometry_processing_time/(float)d->num_of_frames;
    d->average_rasterize_processing_time = d->total_rasterize_processing_time/(float)d->num_of_frames;

}

void init_status(struct render_status *s) {
    s->num_of_frames = 0;
    s->num_of_injected_triangles = 0;
    s->num_of_visible_triangles = 0;
    s->render_fps = 0;
    s->geometry_processing_time = 0;
    s->rasterize_processing_time = 0;

    s->total_injected_triangles = 0;
    s->total_visible_triangles = 0;
    s->total_geometry_processing_time = 0;
    s->total_rasterize_processing_time = 0;

    s->average_injected_triangles = 0;
    s->average_visible_triangles = 0;
    s->average_geometry_processing_time = 0;
    s->average_rasterize_processing_time = 0;
}
