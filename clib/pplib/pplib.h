//=======================================================================
// Project Polyphony
//
// File:
//   pplib.h
//
// Abstract:
//   Graphics library implementation header
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
#ifndef __PPLIB_H__
#define __PPLIB_H__

#include <stdbool.h>

#define MM_PI  3.14159265358979323846F

#define MM_PI_1Z (MM_PI/2.0)
#define MM_PI_2Z MM_PI
#define MM_PI_3Z (MM_PI + MM_PI/2.0)
#define MM_PI_4Z (2.0 *MM_PI)

#define TBL_STEPS 4096
#define PP_MAX_TEXENV 8

// Global registers for analysis
struct render_status {
    unsigned int num_of_frames;
    unsigned int num_of_injected_triangles;
    unsigned int num_of_visible_triangles;
    float render_fps;
    float geometry_processing_time;
    float rasterize_processing_time;

    unsigned int total_injected_triangles;
    unsigned int total_visible_triangles;
    float total_geometry_processing_time;
    float total_rasterize_processing_time;

    unsigned int average_injected_triangles;
    unsigned int average_visible_triangles;
    float average_geometry_processing_time;
    float average_rasterize_processing_time;

};

extern struct render_status rstat_tmp; 
extern struct render_status rstat_out; 


typedef union _fui {
    float f;
    unsigned int ui;
} t_fui;

// depth function
typedef enum {
    LESS,
    ALWAYS,
    NEVER,
    LEQUAL,
    EQUAL,
    GREATER,
    GEQUAL,
    NOTEQUAL
} e_depth_mode;


// texture
typedef enum {
    R8G8B8A8,
    R8G8B8,
    R5G5B5A1,
    R5G6B5,
    R4G4B4A4,
    ETC
} e_texture_type;

typedef enum {
    UNSIGNED_BYTE,
    UNSIGNED_INT_8_8_8_8
} e_texture_pixel_data_type;

typedef enum {
    POINT_SAMPLING,
    BILINEAR
} e_texture_filter;

// texture blender
typedef enum {
    TB_ARG_PRIMARY_COLOR,
    TB_ARG_SECONDARY_COLOR,
    TB_ARG_TEXTURE0,
    TB_ARG_TEXTURE1,
    TB_ARG_CONSTANT
} e_tb_arg;


typedef enum {
    TB_FILTER_RGB,
    TB_FILTER_ONE_MINUS_RGB,
    TB_FILTER_ALPHA,
    TB_FILTER_ONE_MINUS_ALPHA
} e_tb_filter;

typedef enum {
    TB_OP_REPLACE,
    TB_OP_MODULATE,
    TB_OP_ADD,
    TB_OP_ADD_SIGNED,
    TB_OP_INTERPOLATE,
    TB_OP_SUBTRACT
} e_tb_operation;

typedef struct _vtx_buffer {
    unsigned int top_address;
    unsigned int pci_top_address;
    int total_size;
    int num_of_triangles;
} t_vtx_buffer;

typedef struct {
    unsigned int tex_address;
    int width;
    int height;
    e_texture_filter m_tex_min_filter;
    e_texture_filter m_tex_mag_filter;
    e_texture_type m_tex_format;
    e_texture_pixel_data_type m_tex_data_type;
    e_tb_arg m_tb_arg[1][3];
    e_tb_filter m_tb_filter_c[1][3];
    e_tb_filter m_tb_filter_a[1][3];
    e_tb_operation m_tb_operation_c[1];
    e_tb_operation m_tb_operation_a[1];
} s_texture_environment;

extern volatile int render_end;
extern volatile int int_vsync;
extern volatile int dma_end;
extern volatile int render_dma_end;
extern int cur_vtx_gen_bank;
extern int cur_vtx_ras_bank;

extern t_vtx_buffer s_vtx_buffer[2];
extern int cur_color_bank_assign;
extern int render_data_available;
extern int first_vsync;

void set_all_matrix();
void gl_init();
void color_buffer_all_clear();
void ct_set_gold();
void ct_set_silver();


float tcos(float x);
float tsin(float x);

void copy_status(struct render_status *d, struct render_status *s);
void init_status(struct render_status *s);
// hardware control functions
void render_buffered_triangles();
void wait_render_end();
void render_dma_start();
bool render_triangle_available();
void cache_init();
void cache_flush();
void set_tex_enable(bool b);
void set_depth_enable(int func, bool b);

void set_buffer_blend(bool en); 
void swap_vtx_buffer();
void next_render_setting();
void buffer_clear_setting();
void set_3d_reg(unsigned int adrs, unsigned int data);
unsigned int get_current_buffer_ptr();
void set_3d_reg_ptr(unsigned int adrs, unsigned int data, unsigned int ptr);

void backdoor_color_config(int c);

#endif
