//=======================================================================
// Project Polyphony
//
// File:
//   pl_vu.h
//
// Abstract:
//   Vertex processor unit C version
//
//  Created:
//    3 October 2008
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

#ifndef __PL_VU_H__
#define __PL_VU_H__
#include <math.h>
#include "pplib.h"
#include "pl_matrix4.h"
#include "pl_vector4.h"
#include "pl_vertex4.h"
#include "pl_address_table.h"

#define CT_TABLE_SIZE 256

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

typedef enum {
    PL_MODEL_MATRIX,
    PL_VIEW_MATRIX,
    PL_PROJECTION_MATRIX,
    PL_TEXTURE_MATRIX,
    PL_COLOR_MATRIX,
    PL_PALETTE_MATRIX
} vu_e_matrix;

typedef struct {
    int  vnum;    // original array index
    int  *ix, *iy;
   
} vu_s_v;

typedef struct {
    svf_color4     ambient;
    svf_color4     diffuse;
    svf_color4     specular;
    svf_position4  position;
    svf_position3  spot_dir;
    float          spot_exp;
    float          spot_cutoff;
    float          const_att;
    float          linear_att;
    float          quad_att;
} vu_s_light_parameter;

typedef struct {
    svf_color4    ambient;
    svf_color4    diffuse;
    svf_color4    specular;
    svf_color4    emission;
    float          shininess;
} vu_s_light_material;

typedef enum __vu_e_front_face_mode {
    CCW,CW
} vu_e_front_face_mode;

    
typedef enum __vu_e_cull_face_mode {
    BACK,
    FRONT,
    FRONT_AND_BACK
} vu_e_cull_face_mode;

typedef struct {
    svf_vertex       v;
    unsigned int     outcode;
    float            bc[6];
} s_clip_vertex;

// shade mode
typedef enum {
    FLAT,
    SMOOTH,
    COOK_TORRANCE
} e_shade_mode;

// lighting
typedef enum {
    RED,
    GREEN,
    BLUE,
} e_color;

typedef struct {

    // states
    bool m_debug;
    bool m_has_clipped_tri;
    int  m_num_clipped_tri;
    bool m_discard;
    bool has_color;
    bool has_tex;

    int m_vp_x, m_vp_y;
    int m_vp_width, m_vp_height;

    int m_vcnt;               // vertex counter
    svf_triangle s_tri;      // current triangle
    svf_triangle s_tri_c[4]; // generated triangle by clipping
    svf_triangle s_tri_f[4]; // swapped triangle
    // current matrix
    pl_matrix4      pm_stack[PP_CONFIG_MAX_MSTACK];  // projection matrix stack
    pl_matrix4      tm_stack[PP_CONFIG_MAX_MSTACK];  // texture matrix stack
    pl_matrix4      mm_stack[PP_CONFIG_MAX_MSTACK];  // model matrix stack
    pl_matrix4      vm_stack[PP_CONFIG_MAX_MSTACK];  // view matrix stack
    pl_matrix4      cm_stack[PP_CONFIG_MAX_MSTACK];  // color matrix stack
    pl_matrix4                    cur_pm;    // current projection matrix
    pl_matrix4                    cur_tm;    // current texture matrix
    pl_matrix4                    cur_mm;    // current model matrix
    pl_matrix4                    cur_vm;    // current view matrix
    pl_matrix4                    cur_cm;    // current color matrix
    pl_matrix4                    matrix_palette[PP_CONFIG_MAX_MATRIX_PALETTES];
	
    pl_matrix4                    cur_modelview;  // model + view matrix
    pl_matrix4                    cur_modelviewpro;  // model + view + projection matrix
    pl_matrix4*                   cur_matrix;  // current matrix stack 

    // for swapping
    vu_s_v  v_top;
    vu_s_v  v_middle;
    vu_s_v  v_bottom;

    // state originally placed in pp_cu
    // face culling
    vu_e_front_face_mode m_ff_mode;
    vu_e_cull_face_mode m_cf_mode;
    bool m_cull_face_enable;

    bool m_lighting_enable;
    bool m_light_enable[PP_CONFIG_MAX_LIGHTS];
    vu_s_light_parameter m_light_param[PP_CONFIG_MAX_LIGHTS];

    vu_s_light_material m_front_material;
    vu_s_light_material m_back_material;

    svf_color4    m_scene_ambient;
    bool m_light_model_local_viewer;
	e_shade_mode m_shade_mode;
    float m_eta_r, m_eta_g, m_eta_b, m_rough;  // for took-torrance model
    int m_rough_table_size, m_fresnel_r_table_size;
    int m_fresnel_g_table_size,m_fresnel_b_table_size;
    float b_table[CT_TABLE_SIZE +1];   // roughness table 
    float fr_table[CT_TABLE_SIZE +1];  // fresnel red  table 
    float fg_table[CT_TABLE_SIZE +1];  // fresnel green table 
    float fb_table[CT_TABLE_SIZE +1];  // fresnel blue table 
	
    bool  m_dir_flag;
    bool  m_middle_is_in_the_left;
    float m_area;
    float m_area_org;
    // polygon clipper
    int m_vertex_index[7][16]; // plane +1 : final result will be index6
    int m_vertex_index_size[7];
    s_clip_vertex m_v_buffer[16];
    int m_v_buffer_size;
    // draw array
    bool m_vertex_array_en;
    int  m_vertex_array_size;
    float *p_vertex_array;
    bool m_color_array_en;
    int  m_color_array_size;
    float *p_color_array;
    bool m_normal_array_en;
    float *p_normal_array;
    bool m_tex_array_en;
    int  m_tex_array_size;
    float *p_tex_array;
    bool m_weight_array_en;
    int  m_weight_array_size;
    float *p_weight_array;
    bool m_matrix_array_en;
    int  m_matrix_array_size;
    unsigned char *p_matrix_array;	
	
    int m_draw_array_first;
    int m_draw_array_cnt;
    bool m_color_material_en;
    svf_vertex m_da_buffer[PP_VU_DRAW_ARRAY_SIZE];
	// matrix palette
	bool m_matrix_palette_en;
	unsigned int m_matrix_num;
	int m_cur_palette_matrix;
} pl_vu_state;



// graphics functions
void vu_init();
void vu_set_current_matrix0(vu_e_matrix mode, float b[]);
void vu_set_current_matrix1(vu_e_matrix mode, unsigned int b[]);
void vu_set_viewport(unsigned int *data);
void vu_set_vertex4(float x, float y, float z, float w);
void vu_set_color4(float *data);
void vu_set_texcoord2(float *data);
void vu_set_normal3(float *data);
void vu_set_normal3_once(float *data);
void vu_clip_draw(unsigned int v0, unsigned int v1,
                  svf_vertex vb[], int *cp,
                  unsigned int outcode0, unsigned int outcode1,
                  float bc[][6]);
void vu_texture_blender_en(bool en);
void vu_ct_set(int kind);
// new clipper
void vu_clipper_poly();
void vu_gen_outcode(s_clip_vertex *csv, svf_vertex *v); 
void vu_clip_plane(const int plane_no);
void vu_clip_move(const int plane_no, const int idx);
void vu_clip_a_line(const int plane_no, const int idx_v0, const int idx_v1, const bool last_v);
bool vu_is_all_vertex_inside();
bool vu_is_all_vertex_outside();

bool vu_front_face(svf_triangle *p_tri);  // check the polygon front/back
bool vu_cull_face(svf_triangle *p_tri);

void vu_apply_lighting(int pn, int vn);
svf_vertex vu_interpolate(svf_vertex start, svf_vertex end, float t);
void vu_apply_cook_torrance(int pn, int vn);
void vu_gen_roughness(int table_size, float m);
void vu_gen_fresnel(e_color type, int table_size, float c);
float get_roughness(float nh);
float get_fresnel(e_color type, float lh);
float get_microfacet(float nh, float vh, float nl, float nv);
float aexp(float x);
void  vu_color_copy(int pn, int dst, int src);

void vu_set_blend_enable(bool b);

void vu_clear_color_buffer_all();
void vu_clear_color_buffer(bool ms_flag,float *c);
void vu_clear_depth_buffer(bool ms_flag,float c);
void vu_clear_start();
void vu_set_texture(int width, int height, unsigned int adrs,
                    e_texture_type format,  e_texture_pixel_data_type type, void *p);
void vu_set_texture_etc(int width, int height, unsigned int adrs, int size, unsigned int *p);


// vertex transformation
void vu_apply_modelview_matrix(pl_vertex4* v);
void vu_apply_modelview_matrix_once(pl_vertex4* v);
void vu_apply_modelview_matrix_f(float* v);
void vu_apply_projection_matrix(pl_vertex4* v);
void vu_apply_projection_matrix_f(float* v);

void vu_apply_modelview_projection_matrix_f(float* v);
void vu_apply_modelview_projection_matrix_f2(float* dist, float* src);
void vu_apply_matrix_palette_f2(float* dist, float* src, float* pw, unsigned char* pmi);
void vu_apply_matrix_palette_f2_n(float* dist, float* src, float* pw, unsigned char* pmi);

void vu_apply_perspective_division0(pl_vertex4* v);
void vu_apply_perspective_division1(svf_triangle* t);
void vu_apply_perspective_division();
void vu_apply_view_transform0(pl_vertex4* v);
void vu_apply_view_transform1(svf_triangle* t);
void vu_apply_view_transform();
void vu_matrix_prep();

void vu_apply_lighting_tri(int pn);

bool vu_front_face(svf_triangle *p_tri);  // check the polygon front/back
bool vu_cull_face(svf_triangle *p_tri);
void vu_set_multisample(bool en);

float vu_uitof(unsigned int x);
unsigned int vu_ftoui(float f);
unsigned int vu_cnv_f32_to_f22(unsigned int a);
void vu_swap_vertex(int pn, svf_triangle *p_tri);
void vu_triangle_output(int pn) ;
int  vu_round(const float f);

// draw array
void vu_draw_array(int first, int count);
void vu_clipper_poly_draw_array(int block_no, int tri_no);
void vu_apply_modelview_projection_matrix_da(int block_no, int array_size);

#endif
