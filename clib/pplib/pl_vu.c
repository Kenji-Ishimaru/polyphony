//=======================================================================
// Project Polyphony
//
// File:
//   pl_vu.c
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

#include "pl_vu.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

// Global registers for performance analysis
extern unsigned int num_of_frames;
extern unsigned int num_of_injected_triangles;
extern unsigned int num_of_visible_triangles;
float render_fps;
float geometry_processing_time;
float rasterize_processing_time;

pl_vu_state vu_state;

void vu_init() {
  int i,j;
    vu_state.m_debug = false;
    vu_state.m_vp_x = 0;
    vu_state.m_vp_y = 0;
    vu_state.m_vp_width = 640;
    vu_state.m_vp_height = 480;

    vu_state.m_vcnt = 0;
    vu_state.m_has_clipped_tri = false;
    vu_state.m_num_clipped_tri = 0;
    vu_state.m_discard = false;

    vu_state.has_color = false;
    vu_state.has_tex = false;

    // default vertex value
    for (i = 0; i < 3; i++) {
        vu_state.s_tri.v[i].c.r = 0.0;
        vu_state.s_tri.v[i].c.g = 0.0;
        vu_state.s_tri.v[i].c.b = 0.0;
        vu_state.s_tri.v[i].c.a = 1.0;
        vu_state.s_tri.v[i].t.u = 0.0;
        vu_state.s_tri.v[i].t.v = 0.0;
    }
    for (j = 0; j < 4; j++) {
        for (i = 0; i < 3; i++) {
            vu_state.s_tri_c[j].v[i].c.r = 0.0;
            vu_state.s_tri_c[j].v[i].c.g = 0.0;
            vu_state.s_tri_c[j].v[i].c.b = 0.0;
            vu_state.s_tri_c[j].v[i].c.a = 1.0;
            vu_state.s_tri_c[j].v[i].t.u = 0.0;
            vu_state.s_tri_c[j].v[i].t.v = 0.0;
        }
    }

    // face culling
    vu_state.m_cf_mode = BACK;
    vu_state.m_cull_face_enable = false;
    vu_state.m_ff_mode = CCW;
    // material
    vu_state.m_front_material.ambient.r = 0.2;
    vu_state.m_front_material.ambient.g = 0.2;
    vu_state.m_front_material.ambient.b = 0.2;
    vu_state.m_front_material.ambient.a = 1.0;

    vu_state.m_front_material.diffuse.r = 0.8;
    vu_state.m_front_material.diffuse.g = 0.8;
    vu_state.m_front_material.diffuse.b = 0.8;
    vu_state.m_front_material.diffuse.a = 1.0;

    vu_state.m_front_material.specular.r = 0.0;
    vu_state.m_front_material.specular.g = 0.0;
    vu_state.m_front_material.specular.b = 0.0;
    vu_state.m_front_material.specular.a = 1.0;

    vu_state.m_front_material.emission.r = 0.0;
    vu_state.m_front_material.emission.g = 0.0;
    vu_state.m_front_material.emission.b = 0.0;
    vu_state.m_front_material.emission.a = 1.0;

    vu_state.m_front_material.shininess = 0.0;

    vu_state.m_back_material.ambient.r = 0.2;
    vu_state.m_back_material.ambient.g = 0.2;
    vu_state.m_back_material.ambient.b = 0.2;
    vu_state.m_back_material.ambient.a = 1.0;

    vu_state.m_back_material.diffuse.r = 0.8;
    vu_state.m_back_material.diffuse.g = 0.8;
    vu_state.m_back_material.diffuse.b = 0.8;
    vu_state.m_back_material.diffuse.a = 1.0;

    vu_state.m_back_material.specular.r = 0.0;
    vu_state.m_back_material.specular.g = 0.0;
    vu_state.m_back_material.specular.b = 0.0;
    vu_state.m_back_material.specular.a = 1.0;

    vu_state.m_back_material.emission.r = 0.0;
    vu_state.m_back_material.emission.g = 0.0;
    vu_state.m_back_material.emission.b = 0.0;
    vu_state.m_back_material.emission.a = 1.0;

    vu_state.m_back_material.shininess = 0.0;
    // lights
    vu_state.m_lighting_enable = false;

    for (i = 0; i < PP_CONFIG_MAX_LIGHTS;i++) {
        vu_state.m_light_enable[i] = false;
        vu_state.m_light_param[i].ambient.r = 0.0;
        vu_state.m_light_param[i].ambient.g = 0.0;
        vu_state.m_light_param[i].ambient.b = 0.0;
        vu_state.m_light_param[i].ambient.a = 1.0;
        if (i ==0) {
            vu_state.m_light_param[i].diffuse.r = 1.0;
            vu_state.m_light_param[i].diffuse.g = 1.0;
            vu_state.m_light_param[i].diffuse.b = 1.0;
            vu_state.m_light_param[i].specular.r = 1.0;
            vu_state.m_light_param[i].specular.g = 1.0;
            vu_state.m_light_param[i].specular.b = 1.0;
        } else {
            vu_state.m_light_param[i].diffuse.r = 0.0;
            vu_state.m_light_param[i].diffuse.g = 0.0;
            vu_state.m_light_param[i].diffuse.b = 0.0;
            vu_state.m_light_param[i].specular.r = 0.0;
            vu_state.m_light_param[i].specular.g = 0.0;
            vu_state.m_light_param[i].specular.b = 0.0;
        }
        vu_state.m_light_param[i].diffuse.a = 1.0;
        vu_state.m_light_param[i].specular.a = 1.0;

        vu_state.m_light_param[i].position.x = 0.0;
        vu_state.m_light_param[i].position.y = 0.0;
        vu_state.m_light_param[i].position.z = 1.0;
        vu_state.m_light_param[i].position.w = 0.0;
    }
    vu_state.m_scene_ambient.r = 0.2;
    vu_state.m_scene_ambient.g = 0.2;
    vu_state.m_scene_ambient.b = 0.2;
    vu_state.m_scene_ambient.a = 1.0;
    vu_state.m_light_model_local_viewer = false;  // GLES does not support "true"
	vu_state.m_shade_mode = SMOOTH;
    //vu_state.b_table = 0;   // roughness table 
    //vu_state.fr_table = 0;  // fresnel red  table 
    //vu_state.fg_table = 0;  // fresnel green table 
    //vu_state.fb_table = 0;  // fresnel blue table 
    // draw array
    vu_state.m_color_array_en = false;
    vu_state.m_normal_array_en = false;
    vu_state.m_tex_array_en = false;
    vu_state.m_vertex_array_en = false;
    vu_state.m_color_material_en = false;
    // matrix palette
	vu_state.m_matrix_palette_en = false;
	vu_state.m_matrix_num = 0;
	vu_state.m_matrix_array_size = 0;
	vu_state.m_weight_array_en = false;
    vu_state.m_matrix_array_en = false;
	// test
     // gold
    vu_gen_roughness(256,0.35);
    vu_gen_fresnel(RED, 256,0.386);
    vu_gen_fresnel(GREEN, 256,0.433);
    vu_gen_fresnel(BLUE, 256,0.185);
     // silver
    //vu_gen_roughness(256,0.15);
    //vu_gen_fresnel(RED, 256,0.75);
    //vu_gen_fresnel(GREEN, 256,0.75);
    //vu_gen_fresnel(BLUE, 256,0.75);
    PP_SCREEN_MODE = 1;
}

void vu_ct_set(int kind) {
    if (kind == 0) {
        // gold
        vu_gen_roughness(256,0.35);
        vu_gen_fresnel(RED, 256,0.386);
        vu_gen_fresnel(GREEN, 256,0.433);
        vu_gen_fresnel(BLUE, 256,0.185);
	} else {
		// silver
        vu_gen_roughness(256,0.15);
        vu_gen_fresnel(RED, 256,0.75);
        vu_gen_fresnel(GREEN, 256,0.75);
        vu_gen_fresnel(BLUE, 256,0.75);
	}
}

float vu_uitof(unsigned int x) {
    union {
        float f;
        unsigned int ui;
    } u_fui;
    u_fui.ui = x;
    return  u_fui.f;
}

unsigned int vu_cnv_f32_to_f22(unsigned int a) {
    // extract sign
    unsigned int tmp_s;
    int tmp_e;
    unsigned int rbit;
    unsigned int tmp_m;
    tmp_s  = (a >> 31) & 1;
    // extract exp
    tmp_e = (a >> 23) & 0xff;
    tmp_e -= 127;
    tmp_e += 15;
    if (tmp_e < 0) tmp_e = 0;
    // extract fraction
    rbit = (a >> 7) & 1;// bit 7
    tmp_m = (a >> 8)& 0x7fff;
    if (rbit) tmp_m++;
    if (tmp_e != 0) tmp_m |= 0x8000;
    return  (tmp_s << 21) | (tmp_e << 16) | tmp_m;
}


unsigned int vu_ftoui(float f) {
    union {
        float f;
        unsigned int ui;
    } u_fui;
    u_fui.f = f;
#ifdef FLOAT22_INJECTION
    return vu_cnv_f32_to_f22(u_fui.ui);
#endif
    return  u_fui.ui;
}

// graphics functions
void vu_set_current_matrix0(vu_e_matrix mode, float b[]) {
    switch (mode) {
        case PL_MODEL_MATRIX:
            pl_matrix4_set0(&(vu_state.cur_mm), b);
            //vu_state.cur_mm = b;
            break;
        case PL_VIEW_MATRIX:
            pl_matrix4_set0(&(vu_state.cur_vm), b);
            //vu_state.cur_vm = b;
            break;
        case PL_PROJECTION_MATRIX:
            pl_matrix4_set0(&(vu_state.cur_pm), b);
            //vu_state.cur_pm = b;
            break;
        case PL_TEXTURE_MATRIX:
            pl_matrix4_set0(&(vu_state.cur_tm), b);
            //vu_state.cur_tm = b;
            break;
        case PL_COLOR_MATRIX:
            pl_matrix4_set0(&(vu_state.cur_cm), b);
            //vu_state.cur_cm = b;
            break;
		case PL_PALETTE_MATRIX:
            pl_matrix4_set0(&(vu_state.matrix_palette[vu_state.m_matrix_num]), b);
            //vu_state.cur_cm = b;
            break;
        default:
            printf("Unexpected matrix mode.\n");
            break;
    }
}

void vu_set_current_matrix1(vu_e_matrix mode, unsigned int b[]) {
    switch (mode) {
        case PL_MODEL_MATRIX:
            pl_matrix4_set1(&(vu_state.cur_mm), b);
            //vu_state.cur_mm = b;
            break;
        case PL_VIEW_MATRIX:
            pl_matrix4_set1(&(vu_state.cur_vm), b);
            //vu_state.cur_vm = b;
            break;
        case PL_PROJECTION_MATRIX:
            pl_matrix4_set1(&(vu_state.cur_pm), b);
            //vu_state.cur_pm = b;
            break;
        case PL_TEXTURE_MATRIX:
            pl_matrix4_set1(&(vu_state.cur_tm), b);
            //vu_state.cur_tm = b;
            break;
        case PL_COLOR_MATRIX:
            pl_matrix4_set1(&(vu_state.cur_cm), b);
            //vu_state.cur_cm = b;
            break;
		case PL_PALETTE_MATRIX:
            pl_matrix4_set1(&(vu_state.matrix_palette[vu_state.m_matrix_num]), b);
            //vu_state.cur_cm = b;
            break;
        default:
            printf("Unexpected matrix mode.\n");
            break;
    }
}

// vertex transformation
void vu_apply_modelview_matrix(pl_vertex4* v) {
    vu_state.cur_matrix = &(vu_state.cur_mm);
    pl_matrix4_multiply_vertex4(v, vu_state.cur_matrix, v);
    vu_state.cur_matrix = &(vu_state.cur_vm);
    pl_matrix4_multiply_vertex4(v, vu_state.cur_matrix, v);

}

void vu_apply_modelview_matrix_once(pl_vertex4* v) {
    pl_matrix4_multiply_vertex4(v, &(vu_state.cur_modelview), v);
}

void vu_apply_modelview_matrix_f(float* v) {
    pl_matrix4_multiply_f( &(vu_state.cur_modelview), v);
}

void vu_apply_modelview_projection_matrix_f(float* v) {
    pl_matrix4_multiply_f( &(vu_state.cur_modelviewpro), v);
}

void vu_apply_modelview_projection_matrix_f2(float* dist, float* src) {
    pl_matrix4_multiply_f2( &(vu_state.cur_modelviewpro), dist,src);
}

void vu_apply_matrix_palette_f2(float* dist, float* src, float* pw, unsigned char* pmi) {
    int i,j;
	float v[4], vt[4];
	for (i = 0; i < 4; i++) v[i] = 0.0;
	for (i = 0; i < vu_state.m_matrix_array_size; i++) {
        pl_matrix4_multiply_f2( &(vu_state.matrix_palette[*(pmi+i)]), &vt[0],src);
		for (j = 0; j < 4; j++) {
            vt[j] *= *(pw+i);
            v[j] += vt[j];
		}
	}
	for (i = 0; i < 4; i++) *(dist+i) = v[i];
    
}

void vu_apply_matrix_palette_f2_n(float* dist, float* src, float* pw, unsigned char* pmi) {
    int i,j;
	float v[4], vt[4];

	for (i = 0; i < 4; i++) v[i] = 0.0;
	for (i = 0; i < vu_state.m_matrix_array_size; i++) {
        pl_matrix4_multiply_f2_n( &(vu_state.matrix_palette[*(pmi+i)]), &vt[0],src);
		for (j = 0; j < 4; j++) {
            vt[j] *= *(pw+i);
            v[j] += vt[j];
		}
	}
	for (i = 0; i < 4; i++) *(dist+i) = v[i];
    
}

void vu_apply_projection_matrix(pl_vertex4* v) {
    vu_state.cur_matrix = &(vu_state.cur_pm);
    pl_matrix4_multiply_vertex4(v, vu_state.cur_matrix, v);
    if (vu_state.m_debug) {
        printf("clip coordinates =  %f %f %f %f\n",v->x,v->y,v->z,v->w);
    }
}

void vu_apply_projection_matrix_f(float* v) {
    pl_matrix4_multiply_f(&(vu_state.cur_pm), v);
}

void vu_apply_perspective_division0(pl_vertex4* v) {
    if (v->w != 0.0) {
        float iw = 1.0/v->w;

        v->x  = v->x*iw;
        v->y  = v->y*iw;
        v->z  = v->z*iw;
        //v->w  = v->w;      // w is same
    }
    if (vu_state.m_debug)
        printf("n device coordinates =  %f %f %f %f\n",v->x,v->y,v->z,v->w);
}

void vu_apply_perspective_division() {
    int i;
    if (!vu_state.m_has_clipped_tri)
        vu_apply_perspective_division1(&(vu_state.s_tri));
    else {
        for (i=0; i < vu_state.m_num_clipped_tri; i++)
            vu_apply_perspective_division1(&(vu_state.s_tri_c[i]));
    }
}

void vu_apply_perspective_division1(svf_triangle *t) {
    int i;
    float iw;
    for (i = 0; i <3; i++) {
        iw = (t->v[i].coord.element.fw != 0) ? 1.0/t->v[i].coord.element.fw : 0.0;
        t->v[i].coord.element.fx = t->v[i].coord.element.fx * iw;
        t->v[i].coord.element.fy = t->v[i].coord.element.fy * iw;
        t->v[i].coord.element.fz = t->v[i].coord.element.fz * iw;
        //t->v[i].fw = t->v[i].fw;  // w is same
        t->v[i].fiw = iw;
        if (vu_state.m_debug) {
            printf("n device coordinates =  %f %f %f %f\n",t->v[i].coord.element.fx,
            t->v[i].coord.element.fy,t->v[i].coord.element.fz,t->v[i].coord.element.fw);
        }
    }
}


void vu_apply_view_transform0(pl_vertex4* v) {
    float m_05;
    m_05 = 0.5;
    v->x = ((v->x)+1)*vu_state.m_vp_width/2+vu_state.m_vp_x;
    v->y = ((v->y)+1)*vu_state.m_vp_height/2+vu_state.m_vp_y;
    v->z = m_05*v->z + m_05;  // 0 - 1.0
          
    if (vu_state.m_debug) {
        printf("window coordinates = %f %f %f %f\n",v->x,v->y,v->z,v->w);
        printf("coef %f %f %f %f\n",vu_state.m_vp_width,vu_state.m_vp_height,
                                    vu_state.m_vp_x,vu_state.m_vp_y);
    }
}

void vu_apply_view_transform() {
    int i;
    if (!vu_state.m_has_clipped_tri)
        vu_apply_view_transform1(&(vu_state.s_tri));
    else {
        for (i=0; i < vu_state.m_num_clipped_tri; i++)
            vu_apply_view_transform1(&(vu_state.s_tri_c[i]));
    }
}

void vu_apply_view_transform1(svf_triangle *t) {
    int i;
    float m_05;
    m_05 = 0.5;
    for (i = 0; i <3; i++) {
        // converts -1 ~ 1 to 0 ~ height, width
        t->v[i].coord.element.fx = ((t->v[i].coord.element.fx)+1)*(vu_state.m_vp_width)/2+vu_state.m_vp_x;
        t->v[i].coord.element.fy = ((t->v[i].coord.element.fy)+1)*(vu_state.m_vp_height)/2+vu_state.m_vp_y;
        t->v[i].coord.element.fz = m_05*(t->v[i].coord.element.fz) + m_05;  // 0 - 1.0 should support depth range

        if (t->v[i].coord.element.fx > vu_state.m_vp_width) {
            if (vu_state.m_debug)
                printf("over width\n");
            t->v[i].coord.element.fx = (float)vu_state.m_vp_width;
        }
        if (t->v[i].coord.element.fy > vu_state.m_vp_height) {
            if (vu_state.m_debug)
                printf("over height\n");
            t->v[i].coord.element.fy = (float)vu_state.m_vp_height; 
        }

        if (vu_state.m_debug) {
            printf("window coordinates = %f %f %f %f\n",t->v[i].coord.element.fx,
                                                        t->v[i].coord.element.fy,
                                                        t->v[i].coord.element.fz,
                                                        t->v[i].coord.element.fw);
            printf("coeff   = %f %f %f %f\n",(float)vu_state.m_vp_width,
                                           (float)vu_state.m_vp_height,
                                           (float)vu_state.m_vp_x,
                                           (float)vu_state.m_vp_y);
        }
    }
}

void vu_set_viewport(unsigned int *data) {
    vu_state.m_vp_x      = data[0];
    vu_state.m_vp_y      = data[1];
    vu_state.m_vp_width  = data[2];
    vu_state.m_vp_height = data[3];
}


void vu_matrix_prep() {
    // gen model-view matrix
    pl_matrix4_multiply_matrix4(&(vu_state.cur_modelview), &(vu_state.cur_mm), &(vu_state.cur_vm));
    // gen model-view-projection matrix
    pl_matrix4_multiply_matrix4(&(vu_state.cur_modelviewpro), &(vu_state.cur_modelview), &(vu_state.cur_pm));
}

void vu_apply_lighting_tri(int pn) {
    int i;
    if (vu_state.m_shade_mode == FLAT) {
        vu_apply_lighting(pn, 0);
        vu_color_copy(pn, 1,0);
        vu_color_copy(pn, 2,0);
    } else if (vu_state.m_shade_mode == SMOOTH) {
        for (i = 0; i<3; i++) {
            vu_apply_lighting(pn, i);  // calc in eye coordinates
        }
    } else {
        for (i = 0; i<3; i++) {
            vu_apply_cook_torrance(pn, i);  // calc in eye coordinates
        }
    }
}

void  vu_color_copy(int pn, int dst, int src) {
    vu_state.s_tri_f[pn].v[dst].c.r = vu_state.s_tri_f[pn].v[src].c.r;
    vu_state.s_tri_f[pn].v[dst].c.g = vu_state.s_tri_f[pn].v[src].c.g;
    vu_state.s_tri_f[pn].v[dst].c.b = vu_state.s_tri_f[pn].v[src].c.b;
    vu_state.s_tri_f[pn].v[dst].c.a = vu_state.s_tri_f[pn].v[src].c.a;

}

void vu_set_vertex4(float x, float y, float z, float w){
    int i;
    vu_state.s_tri.v[vu_state.m_vcnt].coord.element.fx = x;
    vu_state.s_tri.v[vu_state.m_vcnt].coord.element.fy = y;
    vu_state.s_tri.v[vu_state.m_vcnt].coord.element.fz = z;
    vu_state.s_tri.v[vu_state.m_vcnt].coord.element.fw = w;
    // save original coordinates
    vu_state.s_tri.v[vu_state.m_vcnt].coord_eye.element.fx = x;
    vu_state.s_tri.v[vu_state.m_vcnt].coord_eye.element.fy = y;
    vu_state.s_tri.v[vu_state.m_vcnt].coord_eye.element.fz = z;
    vu_state.s_tri.v[vu_state.m_vcnt].coord_eye.element.fw = w;

    vu_apply_modelview_projection_matrix_f((float*)vu_state.s_tri.v[vu_state.m_vcnt].coord.f);


    if (vu_state.m_vcnt >= 2) {
        vu_clipper_poly();
        if (!vu_state.m_discard) {
            vu_apply_perspective_division();
            vu_apply_view_transform();

            if (!vu_state.m_has_clipped_tri) {
                vu_swap_vertex(0,&(vu_state.s_tri));
                if (!vu_state.m_discard) { // bottom.y 
                    if (vu_cull_face(&(vu_state.s_tri))) {
                        if (vu_state.m_lighting_enable) vu_apply_lighting_tri(0); 
                        vu_triangle_output(0);
                    }
                }
            } else {
                 for (i =0; i < vu_state.m_num_clipped_tri; i++) {
                     vu_swap_vertex(i,&(vu_state.s_tri_c[i]));
                     if (!vu_state.m_discard) { // bottom.y 
                         if (vu_cull_face(&(vu_state.s_tri_c[i]))) {
                             if (vu_state.m_lighting_enable) vu_apply_lighting_tri(i);
                             vu_triangle_output(i);
                         }
                     }
                 }
            }
        }
        vu_state.m_vcnt = 0;
    } else {
        vu_state.m_vcnt++;
    }
}

void vu_triangle_output(int pn) {
    unsigned int t;
    int i;
    unsigned int cur_base, cur_ptr;
    cur_base = s_vtx_buffer[cur_vtx_gen_bank].top_address;
    cur_ptr =  s_vtx_buffer[cur_vtx_gen_bank].total_size;
    for (i = 0; i<3; i++) {
         t = vu_ftoui((float)vu_state.s_tri_f[pn].v[i].wx);
         if (vu_state.m_middle_is_in_the_left) t |= 0x80000000; 
        (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = t;
         cur_ptr++;
        (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui((float)vu_state.s_tri_f[pn].v[i].wy);
         cur_ptr++;
        (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].coord.element.fz*PP_INTERP_BIAS);
         cur_ptr++;
        (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].fiw*PP_INTERP_BIAS);
         cur_ptr++;
        if (vu_state.has_color) {
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].c.r*255.0);
             cur_ptr++;
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].c.g*255.0);
             cur_ptr++;
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].c.b*255.0);
             cur_ptr++;
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].c.a*255.0);
             cur_ptr++;
        }
        if (vu_state.has_tex) {
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].t.u*PP_INTERP_TEX_BIAS);
             cur_ptr++;
            (*(volatile unsigned int  *)(cur_base+cur_ptr*4)) = vu_ftoui(vu_state.s_tri_f[pn].v[i].t.v*PP_INTERP_TEX_BIAS);
             cur_ptr++;
        }
        if (vu_state.m_debug) {
            printf("vtx%d = %f %f %f %f %f %f\n",i,
                    (float)vu_state.s_tri_f[pn].v[i].wx,
                    (float)vu_state.s_tri_f[pn].v[i].wy,
                    vu_state.s_tri_f[pn].v[i].coord.element.fz*PP_INTERP_BIAS,
                    vu_state.s_tri_f[pn].v[i].fiw*PP_INTERP_BIAS,
                    vu_state.s_tri_f[pn].v[i].t.u,
                    vu_state.s_tri_f[pn].v[i].t.v
            );
        }
    }
    s_vtx_buffer[cur_vtx_gen_bank].total_size = cur_ptr;
    s_vtx_buffer[cur_vtx_gen_bank].num_of_triangles++;
}


void vu_swap_vertex(int pn, svf_triangle *p_tri) {
    // temporary set top, middle, bottom
    vu_state.v_top.vnum = 0;    // index0 of p_tri
    vu_state.v_middle.vnum = 1; // index1 of p_tri
    vu_state.v_bottom.vnum = 2; // index2 of p_tri
    vu_state.m_dir_flag = true;
    // set integer y
    p_tri->v[vu_state.v_top.vnum].wy    = vu_round(p_tri->v[vu_state.v_top.vnum].coord.element.fy);
    p_tri->v[vu_state.v_middle.vnum].wy = vu_round(p_tri->v[vu_state.v_middle.vnum].coord.element.fy);
    p_tri->v[vu_state.v_bottom.vnum].wy = vu_round(p_tri->v[vu_state.v_bottom.vnum].coord.element.fy);

    vu_state.v_top.iy     = &(p_tri->v[vu_state.v_top.vnum].wy);
    vu_state.v_middle.iy  = &(p_tri->v[vu_state.v_middle.vnum].wy);
    vu_state.v_bottom.iy =  &(p_tri->v[vu_state.v_bottom.vnum].wy);
    // set integer x
    p_tri->v[vu_state.v_top.vnum].wx =    vu_round(p_tri->v[vu_state.v_top.vnum].coord.element.fx);
    p_tri->v[vu_state.v_middle.vnum].wx = vu_round(p_tri->v[vu_state.v_middle.vnum].coord.element.fx);
    p_tri->v[vu_state.v_bottom.vnum].wx = vu_round(p_tri->v[vu_state.v_bottom.vnum].coord.element.fx);

    vu_state.v_top.ix    = &(p_tri->v[vu_state.v_top.vnum].wx);
    vu_state.v_middle.ix = &(p_tri->v[vu_state.v_middle.vnum].wx);
    vu_state.v_bottom.ix = &(p_tri->v[vu_state.v_bottom.vnum].wx);

    // decide top vertex
    if (*(vu_state.v_top.iy) > *(vu_state.v_middle.iy)) {
        // y0 > y1
        if (*(vu_state.v_middle.iy) > *(vu_state.v_bottom.iy)) {
           // y0 > y1 > y2
           vu_state.v_top.vnum     = 0;
           vu_state.v_middle.vnum  = 1;
           vu_state.v_bottom.vnum  = 2;
        } else if (*(vu_state.v_top.iy) > *(vu_state.v_bottom.iy)) {
            // y0 > y2 > y1
           vu_state.v_top.vnum     = 0;
           vu_state.v_middle.vnum  = 2;
           vu_state.v_bottom.vnum  = 1;
           vu_state.m_dir_flag = false;
        } else {
            // y2 > y0 > y1
           vu_state.v_top.vnum     = 2;
           vu_state.v_middle.vnum  = 0;
           vu_state.v_bottom.vnum  = 1;
        }
    } else {
        // y1 > y0
        if (*(vu_state.v_top.iy) > *(vu_state.v_bottom.iy)) {
            // y1 > y0 > y2
           vu_state.v_top.vnum     = 1;
           vu_state.v_middle.vnum  = 0;
           vu_state.v_bottom.vnum  = 2;
           vu_state.m_dir_flag = false;
        } else if  (*(vu_state.v_middle.iy) > *(vu_state.v_bottom.iy)) {
            // y1 > y2 > y0
           vu_state.v_top.vnum     = 1;
           vu_state.v_middle.vnum  = 2;
           vu_state.v_bottom.vnum  = 0;
        } else {
            // y2 > y1 > y0
           vu_state.v_top.vnum     = 2;
           vu_state.v_middle.vnum  = 1;
           vu_state.v_bottom.vnum  = 0;
           vu_state.m_dir_flag = false;
        }
    }

    // v[0] = top
    // v[1] = middle
    // v[2] = bottom
    vu_state.s_tri_f[pn].v[0] = p_tri->v[vu_state.v_top.vnum];
    vu_state.s_tri_f[pn].v[1] = p_tri->v[vu_state.v_middle.vnum];
    vu_state.s_tri_f[pn].v[2] = p_tri->v[vu_state.v_bottom.vnum];
    // discard bottom.y == screen heiht) triangle
    if (vu_state.s_tri_f[pn].v[2].wy == vu_state.m_vp_height) {
        vu_state.m_discard = true;
    } else  vu_state.m_discard = false;
}

int vu_round(float f) {
    int i;
    float f_05;
    f_05 = 0.5;
    i = (int)(f + f_05);
    return i;
}

void vu_set_color4(float *data){
    vu_state.s_tri.v[vu_state.m_vcnt].c.r = data[0];
    vu_state.s_tri.v[vu_state.m_vcnt].c.g = data[1];
    vu_state.s_tri.v[vu_state.m_vcnt].c.b = data[2];
    vu_state.s_tri.v[vu_state.m_vcnt].c.a = data[3];
}

void vu_set_texcoord2(float *data){
    vu_state.s_tri.v[vu_state.m_vcnt].t.u = data[0];
    vu_state.s_tri.v[vu_state.m_vcnt].t.v = data[1];
}

void vu_set_normal3(float *data){
    // model/view transform
    pl_vertex4 v;
    v.x = data[0];
    v.y = data[1];
    v.z = data[2];
    v.w = 0.0;

    vu_state.cur_matrix = &(vu_state.cur_mm);
    pl_matrix4_multiply_vertex4(&v, vu_state.cur_matrix, &v);

    vu_state.cur_matrix = &(vu_state.cur_vm);
    pl_matrix4_multiply_vertex4(&v, vu_state.cur_matrix, &v);
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fx = v.x;
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fy = v.y;
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fz = v.z;
}

void vu_set_normal3_once(float *data){
    // just set vector
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fx = data[0];
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fy = data[1];
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fz = data[2];
    vu_state.s_tri.v[vu_state.m_vcnt].n.element.fw = 0.0;
}

void vu_apply_cook_torrance(int pn, int vn) {
    svf_color4 primary_color, secondary_color;
    float nl[PP_CONFIG_MAX_LIGHTS];
    svf_position3 h;
    bool  sc_flag;
    float nh, vh, nv, lh;
	int i;
    // view vector in eye coordinates
    vu_apply_modelview_matrix_f((float*)vu_state.s_tri_f[pn].v[vn].coord_eye.f);

    svf_position3 view;
    view.x = 0 - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fx;
    view.y = 0 - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fy;
    view.z = 0 - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fz;
    // normal transform
    vu_apply_modelview_matrix_f((float*)vu_state.s_tri_f[pn].v[vn].n.f);

    float d = sqrt(view.x*view.x + view.y*view.y + view.z*view.z);
    if (d != 0) {
        view.x /= d; view.y /= d; view.z /= d;
    }
    // normal vector
    d = sqrt(vu_state.s_tri_f[pn].v[vn].n.element.fx * vu_state.s_tri_f[pn].v[vn].n.element.fx +
             vu_state.s_tri_f[pn].v[vn].n.element.fy * vu_state.s_tri_f[pn].v[vn].n.element.fy +
             vu_state.s_tri_f[pn].v[vn].n.element.fz * vu_state.s_tri_f[pn].v[vn].n.element.fz);
    if (d != 0) {
        vu_state.s_tri_f[pn].v[vn].n.element.fx /= d;
        vu_state.s_tri_f[pn].v[vn].n.element.fy /= d;
        vu_state.s_tri_f[pn].v[vn].n.element.fz /= d;
    }
    // light vector
    svf_position3 lv[PP_CONFIG_MAX_LIGHTS]; 
    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
        if (vu_state.m_light_enable[i]) {
            lv[i].x = vu_state.m_light_param[i].position.x - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fx;
            lv[i].y = vu_state.m_light_param[i].position.y - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fy;
            lv[i].z = vu_state.m_light_param[i].position.z - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fz;
            d = sqrt(lv[i].x * lv[i].x + lv[i].y * lv[i].y + lv[i].z * lv[i].z);
            if (d != 0) {
                lv[i].x /= d; lv[i].y /= d; lv[i].z /= d;
            }
        }
    }
    // primary color
    float material_diffuse;
    primary_color.r  = vu_state.m_front_material.emission.r;
    primary_color.r += vu_state.m_front_material.ambient.r * vu_state.m_scene_ambient.r;
    primary_color.g  = vu_state.m_front_material.emission.g;
    primary_color.g += vu_state.m_front_material.ambient.g * vu_state.m_scene_ambient.g;
    primary_color.b  = vu_state.m_front_material.emission.b;
    primary_color.b += vu_state.m_front_material.ambient.b * vu_state.m_scene_ambient.b;

    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
        if (vu_state.m_light_enable[i]) {
            nl[i]    = vu_state.s_tri_f[pn].v[vn].n.element.fx * lv[i].x + 
                       vu_state.s_tri_f[pn].v[vn].n.element.fy * lv[i].y +
                       vu_state.s_tri_f[pn].v[vn].n.element.fz * lv[i].z;
            if (nl[i] < 0.0) nl[i] = 0.0;
            // red
            if (vu_state.m_color_material_en) material_diffuse = vu_state.s_tri_f[pn].v[vn].c.r;
            else material_diffuse = vu_state.m_front_material.diffuse.r;
            primary_color.r += vu_state.m_front_material.ambient.r * vu_state.m_light_param[i].ambient.r +
                             nl[i] * material_diffuse * vu_state.m_light_param[i].diffuse.r;
            // green
            if (vu_state.m_color_material_en) material_diffuse = vu_state.s_tri_f[pn].v[vn].c.g;
            else material_diffuse = vu_state.m_front_material.diffuse.g;
            primary_color.g += vu_state.m_front_material.ambient.g * vu_state.m_light_param[i].ambient.g +
                             nl[i] * material_diffuse * vu_state.m_light_param[i].diffuse.g;

            // blue
            if (vu_state.m_color_material_en) material_diffuse = vu_state.s_tri_f[pn].v[vn].c.b;
            else material_diffuse = vu_state.m_front_material.diffuse.b;
            primary_color.b += vu_state.m_front_material.ambient.b * vu_state.m_light_param[i].ambient.b +
                             nl[i] * material_diffuse * vu_state.m_light_param[i].diffuse.b;
        }
    }
    if (primary_color.r > 1.0) primary_color.r = 1.0; 
    if (primary_color.g > 1.0) primary_color.g = 1.0; 
    if (primary_color.b > 1.0) primary_color.b = 1.0; 

    // secondary color
    secondary_color.r = 0;
    secondary_color.g = 0;
    secondary_color.b = 0;
    if ((vu_state.m_front_material.specular.r == 0.0)&&
        (vu_state.m_front_material.specular.g == 0.0)&&
        (vu_state.m_front_material.specular.b == 0.0)) sc_flag = false;
    else sc_flag = true;

    if (sc_flag) {
        for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
            if (vu_state.m_light_enable[i]) {  
                if (nl[i] > 0.0) {
                    if (vu_state.m_light_model_local_viewer) {
                        h.x = lv[i].x + view.x;
                        h.y = lv[i].y + view.y;
                        h.z = lv[i].z + view.z;
                    } else {
                        h.x = lv[i].x + 0.0;
                        h.y = lv[i].y + 0.0;
                        h.z = lv[i].z + 1.0;
                    }
                    // normalize
                    d = sqrt(h.x * h.x + h.y * h.y + h.z * h.z);
                    if (d != 0) {
                        h.x /= d;
                        h.y /= d;
                        h.z /= d;
                    }
                    // dp
                    nh = vu_state.s_tri_f[pn].v[vn].n.element.fx * h.x +
                         vu_state.s_tri_f[pn].v[vn].n.element.fy * h.y +
                         vu_state.s_tri_f[pn].v[vn].n.element.fz * h.z;

                    vh = view.x * h.x +
                         view.y * h.y +
                         view.z * h.z;

                    nv = vu_state.s_tri_f[pn].v[vn].n.element.fx * view.x +
                         vu_state.s_tri_f[pn].v[vn].n.element.fy * view.y +
                         vu_state.s_tri_f[pn].v[vn].n.element.fz * view.z;

                    lh = lv[i].x * h.x +
                         lv[i].y * h.y +
                         lv[i].z * h.z;

                    secondary_color.r +=  get_fresnel(RED,lh)*(get_roughness(nh)*get_microfacet(nh,vh,nl[i],nv)/nv);
                    secondary_color.g +=  get_fresnel(GREEN,lh)*(get_roughness(nh)*get_microfacet(nh,vh,nl[i],nv)/nv);
                    secondary_color.b +=  get_fresnel(BLUE,lh)*(get_roughness(nh)*get_microfacet(nh,vh,nl[i],nv)/nv);
                }
            }
        }
    }
    if (secondary_color.r > 1.0) secondary_color.r = 1.0; 
    if (secondary_color.g > 1.0) secondary_color.g = 1.0; 
    if (secondary_color.b > 1.0) secondary_color.b = 1.0; 

    vu_state.s_tri_f[pn].v[vn].c.r = primary_color.r + secondary_color.r;
    vu_state.s_tri_f[pn].v[vn].c.g = primary_color.g + secondary_color.g;
    vu_state.s_tri_f[pn].v[vn].c.b = primary_color.b + secondary_color.b;
    if (vu_state.s_tri_f[pn].v[vn].c.r > 1.0) vu_state.s_tri_f[pn].v[vn].c.r = 1.0;
    if (vu_state.s_tri_f[pn].v[vn].c.g > 1.0) vu_state.s_tri_f[pn].v[vn].c.g = 1.0;
    if (vu_state.s_tri_f[pn].v[vn].c.b > 1.0) vu_state.s_tri_f[pn].v[vn].c.b = 1.0;

    if (vu_state.m_color_material_en) material_diffuse = vu_state.s_tri_f[pn].v[vn].c.a;
    else material_diffuse = vu_state.m_front_material.diffuse.a;
    vu_state.s_tri_f[pn].v[vn].c.a = material_diffuse;
}

void vu_gen_roughness(int table_size, float m) {
	int i;
    float v, cnh;
    float step;
    float e;
    step = 1.0/table_size;
    vu_state.m_rough = m;

    vu_state.m_rough_table_size = table_size;
    cnh = 0.0;
    for (i = 0; i<vu_state.m_rough_table_size; i++) {
        if (i < 1) v = 0.0;
        else {
            e = (cnh*cnh-1.0)/(vu_state.m_rough*vu_state.m_rough*cnh*cnh);
            v = 1.0/(4.0*3.14*vu_state.m_rough*vu_state.m_rough*/*pow(cnh,4)*/cnh*cnh*cnh*cnh)
                *aexp(e);
        }
        vu_state.b_table[i] = v;
        cnh += step;
    }
    // final value
    cnh = 1.0;
    e = (cnh*cnh-1.0)/(vu_state.m_rough*vu_state.m_rough*cnh*cnh);
    v = 1.0/(4.0*3.14*vu_state.m_rough*vu_state.m_rough*/*pow(cnh,4)*/cnh*cnh*cnh*cnh)
        *aexp(e);
    vu_state.b_table[vu_state.m_rough_table_size] =v ;

}

float aexp(float x) {
    //ex = 1.0/n!*x^n
	int i, flag;
	float ex, nn, xn;
	xn = 1.0;
	nn = 1.0;
	ex = 1.0;
    flag = 0;
    if (x ==0.0) ex = 1.0;
    else {
        if (x < 0.0) {
            x = -x;
            flag = 1;
        }
    	for (i=1;i <20; i++) {
	    	nn = nn * (float)i;
	    	xn = xn *x;
	    	ex += xn/nn;
	    }
        if (flag) ex = 1.0/ex;
    }
	return ex;
}


void vu_gen_fresnel(e_color type, int table_size, float c) {
    int i;
    float e;
    float v, clh;
    float g, ga,gb;
    float step = 1.0/table_size;
    e = (1.0+sqrt(c))/(1.0-sqrt(c));
    clh = 0.0;

    switch(type) {
        case RED:
            vu_state.m_eta_r = e;
            vu_state.m_fresnel_r_table_size = table_size;

            for (i=0; i<vu_state.m_fresnel_r_table_size; i++) {
                g = sqrt(e*e-1.0+clh*clh);
                ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
                gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
                v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
                vu_state.fr_table[i] = v;
                clh += step;
            }
            clh = 1.0;
            g = sqrt(e*e-1.0+clh*clh);
            ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
            gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
            v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
            vu_state.fr_table[vu_state.m_fresnel_r_table_size] = v;
            break;
        case GREEN:
            vu_state.m_eta_g = e;
            vu_state.m_fresnel_g_table_size = table_size;

            for (i=0; i<vu_state.m_fresnel_g_table_size; i++) {
                g = sqrt(e*e-1.0+clh*clh);
                ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
                gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
                v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
                vu_state.fg_table[i] = v;
                clh += step;
            }
            clh = 1.0;
            g = sqrt(e*e-1.0+clh*clh);
            ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
            gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
            v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
            vu_state.fg_table[vu_state.m_fresnel_g_table_size] = v; 
            break;
        case BLUE:
            vu_state.m_eta_b = e;
            vu_state.m_fresnel_b_table_size = table_size;

            for (i=0; i<vu_state.m_fresnel_b_table_size; i++) {
                g = sqrt(e*e-1.0+clh*clh);
                ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
                gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
                v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
                vu_state.fb_table[i] = v;
                clh += step;
            }
            clh = 1.0;
            g = sqrt(e*e-1.0+clh*clh);
            ga = ((clh*(g+clh)-1.0)*(clh*(g+clh)-1.0));
            gb = ((clh*(g+clh)+1.0)*(clh*(g+clh)+1.0));
            v = 0.5*((g-clh)*(g-clh)/(g+clh)*(g+clh))*((ga/gb) + 1.0);
            vu_state.fb_table[vu_state.m_fresnel_b_table_size] = v; 

            break;
    }
}
float get_roughness(float nh) {
    float r,step;
    float min, max,t, tr;
    int ti;
    step = 1.0 / (float)vu_state.m_rough_table_size;
    t = nh/step;
    ti = (int)t;
    tr = t - (float)ti;
    min = vu_state.b_table[ti];
    max = vu_state.b_table[ti+1];
    r = tr*min + (1.0-tr)*max;

    return r;
}
float get_fresnel(e_color type, float lh) {
    float f;
    float step;
    float min, max,t, tr;
    int ti;
    switch (type) {
        case RED:
            step = 1.0 / (float)vu_state.m_fresnel_r_table_size;
            t = lh/step;
            ti = (int)t;
            tr = t - (float)ti;
            min = vu_state.fr_table[ti];
            max = vu_state.fr_table[ti+1];
            f = tr*min + (1.0-tr)*max;
            break;
        case GREEN:
            step = 1.0 / (float)vu_state.m_fresnel_g_table_size;
            t = lh/step;
            ti = (int)t;
            tr = t - (float)ti;
            min = vu_state.fg_table[ti];
            max = vu_state.fg_table[ti+1];
            f = tr*min + (1.0-tr)*max;
            break;
        case BLUE:
            step = 1.0 / (float)vu_state.m_fresnel_b_table_size;
            t = lh/step;
            ti = (int)t;
            tr = t - (float)ti;
            min = vu_state.fb_table[ti];
            max = vu_state.fb_table[ti+1];
            f = tr*min + (1.0-tr)*max;
            break;
    }
    return f;
}

float get_microfacet(float nh, float vh, float nl, float nv) {
    float r,g1, g2;
    if ((nl < 0.0) | (nv < 0.0) | (nh < 0.0) | (vh < 0.0)) r = 0.0;
    else {
        g1 = 2.0*nh*nv/vh;
        g2 = 2.0*nh*nl/vh;
        if (1.0 < g1) {
            if (1.0 < g2) r = 1.0; 
            else r = g2;
        } else {
            if (g1 < g2) r = g1; 
            else r = g2;
        }
    }
    return r;
}

void vu_apply_lighting(int pn, int vn) {
    int i;
    svf_color4 primary_color, secondary_color;
    float nl[PP_CONFIG_MAX_LIGHTS];
    svf_position3 view;
    svf_position3 h;
    float nh, nhp;
    float material_specular;
    bool  sc_flag;

    // view vector in eye coordinates
    if (!vu_state.m_matrix_palette_en) {
        vu_apply_modelview_matrix_f((float*)vu_state.s_tri_f[pn].v[vn].coord_eye.f);
	}
    view.x = -(vu_state.s_tri_f[pn].v[vn].coord_eye.element.fx);
    view.y = -(vu_state.s_tri_f[pn].v[vn].coord_eye.element.fy);
    view.z = -(vu_state.s_tri_f[pn].v[vn].coord_eye.element.fz);

    // normal transform
    if (!vu_state.m_matrix_palette_en) {
        vu_apply_modelview_matrix_f((float*)vu_state.s_tri_f[pn].v[vn].n.f);
	}
    float d = sqrt(view.x*view.x + view.y*view.y + view.z*view.z);
    float id;
    if (d != 0) {
        id = 1.0/d;
        view.x *= id; view.y *= id; view.z *= id;
    }
    // normal vector
    d = sqrt(vu_state.s_tri_f[pn].v[vn].n.element.fx * vu_state.s_tri_f[pn].v[vn].n.element.fx +
             vu_state.s_tri_f[pn].v[vn].n.element.fy * vu_state.s_tri_f[pn].v[vn].n.element.fy +
             vu_state.s_tri_f[pn].v[vn].n.element.fz * vu_state.s_tri_f[pn].v[vn].n.element.fz);
    if (d != 0) {
        id = 1.0/d;
        vu_state.s_tri_f[pn].v[vn].n.element.fx *= id;
        vu_state.s_tri_f[pn].v[vn].n.element.fy *= id;
        vu_state.s_tri_f[pn].v[vn].n.element.fz *= id;
    }
    // light vector
    svf_position3 lv[PP_CONFIG_MAX_LIGHTS]; 
    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
        if (vu_state.m_light_enable[i]) {
            lv[i].x = vu_state.m_light_param[i].position.x - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fx;
            lv[i].y = vu_state.m_light_param[i].position.y - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fy;
            lv[i].z = vu_state.m_light_param[i].position.z - vu_state.s_tri_f[pn].v[vn].coord_eye.element.fz;
            d = sqrt(lv[i].x * lv[i].x + lv[i].y * lv[i].y + lv[i].z * lv[i].z);
            if (d != 0) {
                 id = 1.0/d;
                lv[i].x *= id; lv[i].y *= id; lv[i].z *= id;
            }
        }
    }
    // N.L dot product
    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
        if (vu_state.m_light_enable[i]) {
            nl[i] = vu_state.s_tri_f[pn].v[vn].n.element.fx * lv[i].x + 
                    vu_state.s_tri_f[pn].v[vn].n.element.fy * lv[i].y +
                    vu_state.s_tri_f[pn].v[vn].n.element.fz * lv[i].z;
            if (nl[i] < 0) nl[i] = 0;
        }
    }
    // primary color
    float material_diffuse_r, material_diffuse_g, material_diffuse_b, material_diffuse_a;
    float material_ambient_r, material_ambient_g, material_ambient_b, material_ambient_a;
    if (vu_state.m_color_material_en) {
        material_diffuse_r = vu_state.s_tri_f[pn].v[vn].c.r;
        material_ambient_r = vu_state.s_tri_f[pn].v[vn].c.r;
    } else {
        material_diffuse_r = vu_state.m_front_material.diffuse.r;
        material_ambient_r = vu_state.m_front_material.diffuse.r;
    }
    if (vu_state.m_color_material_en) {
        material_diffuse_g = vu_state.s_tri_f[pn].v[vn].c.g;
        material_ambient_g = vu_state.s_tri_f[pn].v[vn].c.g;
    } else {
        material_diffuse_g = vu_state.m_front_material.diffuse.g;
        material_ambient_g = vu_state.m_front_material.diffuse.g;
    }
    if (vu_state.m_color_material_en) {
        material_diffuse_b = vu_state.s_tri_f[pn].v[vn].c.b;
        material_ambient_b = vu_state.s_tri_f[pn].v[vn].c.b;
    } else {
        material_diffuse_b = vu_state.m_front_material.diffuse.b;
        material_ambient_b = vu_state.m_front_material.diffuse.b;
    }
    if (vu_state.m_color_material_en) {
        material_diffuse_a = vu_state.s_tri_f[pn].v[vn].c.a;
        material_ambient_a = vu_state.s_tri_f[pn].v[vn].c.a;
    } else {
        material_diffuse_a = vu_state.m_front_material.diffuse.a;
        material_ambient_a = vu_state.m_front_material.diffuse.a;
    }

    primary_color.r  = vu_state.m_front_material.emission.r;
    primary_color.r += vu_state.m_front_material.ambient.r * vu_state.m_scene_ambient.r;
    primary_color.g  = vu_state.m_front_material.emission.g;
    primary_color.g += vu_state.m_front_material.ambient.g * vu_state.m_scene_ambient.g;
    primary_color.b  = vu_state.m_front_material.emission.b;
    primary_color.b += vu_state.m_front_material.ambient.b * vu_state.m_scene_ambient.b;
    for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
        if (vu_state.m_light_enable[i]) {
            primary_color.r += material_ambient_r * vu_state.m_light_param[i].ambient.r +
                               nl[i] * material_diffuse_r * vu_state.m_light_param[i].diffuse.r;
            primary_color.g += material_ambient_g * vu_state.m_light_param[i].ambient.g +
                               nl[i] * material_diffuse_g * vu_state.m_light_param[i].diffuse.g;
            primary_color.b += material_ambient_b * vu_state.m_light_param[i].ambient.b +
                               nl[i] * material_diffuse_b * vu_state.m_light_param[i].diffuse.b;
        }
    }
    if (primary_color.r > 1.0) primary_color.r = 1.0; 
    if (primary_color.g > 1.0) primary_color.g = 1.0; 
    if (primary_color.b > 1.0) primary_color.b = 1.0; 

    // secondary color
    secondary_color.r = 0;
    secondary_color.g = 0;
    secondary_color.b = 0;

    if ((vu_state.m_front_material.specular.r == 0.0)&&
        (vu_state.m_front_material.specular.g == 0.0)&&
        (vu_state.m_front_material.specular.b == 0.0)) sc_flag = false;
    else sc_flag = true;

    if (sc_flag) {
        for (i = 0; i < PP_CONFIG_MAX_LIGHTS; i++) {
            if (vu_state.m_light_enable[i]) {  
                if (nl[i] > 0.0) {
                    if (vu_state.m_light_model_local_viewer) {
                        h.x = lv[i].x + view.x;
                        h.y = lv[i].y + view.y;
                        h.z = lv[i].z + view.z;
                    } else {
                        h.x = lv[i].x + 0.0;
                        h.y = lv[i].y + 0.0;
                        h.z = lv[i].z + 1.0;
                    }
                    // normalize
                    d = sqrt(h.x * h.x + h.y * h.y + h.z * h.z);
                    if (d != 0.0) {
                        h.x /= d;
                        h.y /= d;
                        h.z /= d;
                    }
                    // dp
                    nh = vu_state.s_tri_f[pn].v[vn].n.element.fx * h.x +
                         vu_state.s_tri_f[pn].v[vn].n.element.fy * h.y +
                         vu_state.s_tri_f[pn].v[vn].n.element.fz * h.z;
                    // Sclick approximation
                    nhp = nh / (vu_state.m_front_material.shininess -
                                vu_state.m_front_material.shininess*nh + nh);
                    material_specular = vu_state.m_front_material.specular.r;
                    secondary_color.r +=  (nhp * material_specular * vu_state.m_light_param[i].specular.r);
                    material_specular = vu_state.m_front_material.specular.g;
                    secondary_color.g +=  (nhp * material_specular * vu_state.m_light_param[i].specular.g);
                    material_specular = vu_state.m_front_material.specular.b;
                    secondary_color.b +=  (nhp * material_specular * vu_state.m_light_param[i].specular.b);
                }
            }
        }
    }
    if (secondary_color.r > 1.0) secondary_color.r = 1.0; 
    if (secondary_color.g > 1.0) secondary_color.g = 1.0; 
    if (secondary_color.b > 1.0) secondary_color.b = 1.0; 
    vu_state.s_tri_f[pn].v[vn].c.r = primary_color.r + secondary_color.r;
    vu_state.s_tri_f[pn].v[vn].c.g = primary_color.g + secondary_color.g;
    vu_state.s_tri_f[pn].v[vn].c.b = primary_color.b + secondary_color.b;
    if (vu_state.s_tri_f[pn].v[vn].c.r > 1.0) vu_state.s_tri_f[pn].v[vn].c.r = 1.0;
    if (vu_state.s_tri_f[pn].v[vn].c.g > 1.0) vu_state.s_tri_f[pn].v[vn].c.g = 1.0;
    if (vu_state.s_tri_f[pn].v[vn].c.b > 1.0) vu_state.s_tri_f[pn].v[vn].c.b = 1.0;

    vu_state.s_tri_f[pn].v[vn].c.a = material_diffuse_a;
}

void vu_clip_draw(unsigned int v0, unsigned int v1,
                  svf_vertex vb[], int *cp,
                  unsigned int outcode0, unsigned int outcode1,
                  float bc[][6]) {
    int i;
    float a, a0, a1;
    svf_vertex v_p;
    if ((outcode0 & outcode1) == 0) {  // at least one is inside
        if ((outcode0 | outcode1) == 0) { // both inside
            vb[(*cp)++] = vu_state.s_tri.v[v1];
        } else {
            // gen new point
            unsigned int clip = outcode0 | outcode1;
            unsigned int mask = 1;
            a0 = 0.0; a1 = 1.0;
            for(i = 0; i <6; i++) {
                if ((clip & mask) != 0) {
                    a = bc[v0][5-i]/(bc[v0][5-i] - bc[v1][5-i]);
                    if ((outcode0 & mask) !=0)
                        a0 = max(a0, a);  // p0 is outside
                    else
                        a1 = min(a1, a);  // p1 is outside
                    if (a1<a0) break;  // reject
                }
                mask = mask << 1;
            }
            if (a1 >= a0) {
                vu_state.m_has_clipped_tri = true;
                if (outcode0 !=0) {   // new p0
                    v_p = vu_interpolate(vu_state.s_tri.v[v0], vu_state.s_tri.v[v1], a0);
                    vb[(*cp)++] = v_p;
                }
                if (outcode1 !=0) {   // new p1
                    v_p = vu_interpolate(vu_state.s_tri.v[v0], vu_state.s_tri.v[v1], a1);
                    vb[(*cp)++] = v_p;
                } else vb[(*cp)++] = vu_state.s_tri.v[v1];
            }
        }
    }
}

svf_vertex vu_interpolate(svf_vertex start,
                          svf_vertex end, float t) {
    svf_vertex r;
    r.coord.element.fx = start.coord.element.fx +  (t * (end.coord.element.fx - start.coord.element.fx));
    r.coord.element.fy = start.coord.element.fy +  (t * (end.coord.element.fy - start.coord.element.fy));
    r.coord.element.fz = start.coord.element.fz +  (t * (end.coord.element.fz - start.coord.element.fz));
    r.coord.element.fw = start.coord.element.fw +  (t * (end.coord.element.fw - start.coord.element.fw));
    // eye coordinates
    r.coord_eye.element.fx = start.coord_eye.element.fx +  (t * (end.coord_eye.element.fx - start.coord_eye.element.fx));
    r.coord_eye.element.fy = start.coord_eye.element.fy +  (t * (end.coord_eye.element.fy - start.coord_eye.element.fy));
    r.coord_eye.element.fz = start.coord_eye.element.fz +  (t * (end.coord_eye.element.fz - start.coord_eye.element.fz));
    r.coord_eye.element.fw = start.coord_eye.element.fw +  (t * (end.coord_eye.element.fw - start.coord_eye.element.fw));
    // color 
    r.c.r = start.c.r +  (t * (end.c.r - start.c.r));
    r.c.g = start.c.g +  (t * (end.c.g - start.c.g));
    r.c.b = start.c.b +  (t * (end.c.b - start.c.b));
    r.c.a = start.c.a +  (t * (end.c.a - start.c.a));
    // normal 
    r.n.element.fx = start.n.element.fx +  (t * (end.n.element.fx - start.n.element.fx));
    r.n.element.fy = start.n.element.fy +  (t * (end.n.element.fy - start.n.element.fy));
    r.n.element.fz = start.n.element.fz +  (t * (end.n.element.fz - start.n.element.fz));
    r.n.element.fw = start.n.element.fw +  (t * (end.n.element.fw - start.n.element.fw));
    // texture
    r.t.u = start.t.u +  (t * (end.t.u - start.t.u));
    r.t.v = start.t.v +  (t * (end.t.v - start.t.v));

    return r;
}


bool vu_front_face(svf_triangle *p_tri) {
    // true  : front facing
    // false : back facing
    vu_state.m_area= (p_tri->v[vu_state.v_top.vnum].wx-p_tri->v[vu_state.v_bottom.vnum].wx)*
                     (p_tri->v[vu_state.v_middle.vnum].wy-p_tri->v[vu_state.v_bottom.vnum].wy)-
                     (p_tri->v[vu_state.v_top.vnum].wy-p_tri->v[vu_state.v_bottom.vnum].wy)*
                     (p_tri->v[vu_state.v_middle.vnum].wx-p_tri->v[vu_state.v_bottom.vnum].wx);
    vu_state.m_area_org = vu_state.m_area;
    vu_state.m_middle_is_in_the_left = (vu_state.m_area > 0) ? true : false;
    if (vu_state.m_ff_mode == CW) vu_state.m_area = -vu_state.m_area;
    if (!vu_state.m_dir_flag) vu_state.m_area = -vu_state.m_area;
    if (vu_state.m_area <= 0) return false;  // back face
    else return true;  // front face
}

bool vu_cull_face(svf_triangle *p_tri) {
    // true : not culled, rasterized
    // false: culled, not rasterized
    if (vu_state.m_cull_face_enable) {
        if (vu_state.m_cf_mode == BACK) {
            if (vu_front_face(p_tri)) return true; // front face is not culled
            else return false;                     // back face is culled
        } else if (vu_state.m_cf_mode == FRONT) {
            if (vu_front_face(p_tri)) return false;// front face is culled
            else return true;                      // back face is not culled
        } else  return false;                      // FRONT_AND_BACK
    } else {
        // call front_fase() to get area information
        vu_front_face(p_tri);
        return true;  // always pass
    }
}

void vu_clipper_poly() {
    int i;
    vu_state.m_has_clipped_tri = false;
    vu_state.m_discard = false;
    vu_state.m_num_clipped_tri = 0;

    // initialize index size 
    vu_state.m_v_buffer_size = 0;
    for (i = 0; i<7; i++)
        vu_state.m_vertex_index_size[i] = 0;
 
    // calculate outcodes for original triangles
    s_clip_vertex *p_csv;
    for (i=0; i<3; i++) {
        vu_state.m_v_buffer[i].v = vu_state.s_tri.v[i]; // copy triangle vertices
        p_csv = &(vu_state.m_v_buffer[i]);
        vu_gen_outcode(p_csv,&(vu_state.m_v_buffer[i].v));
        vu_state.m_vertex_index[0][i] = vu_state.m_v_buffer_size;
        vu_state.m_v_buffer_size++;
        vu_state.m_vertex_index_size[0]++;
    }

    if (vu_is_all_vertex_inside()) {
        vu_state.m_has_clipped_tri = false;
    } else if (vu_is_all_vertex_outside()) {
        vu_state.m_discard = true;   // will be not rendered
    } else {
        // do clipping 
        for (i = 0; i <6; i++) {  // per clipping plane
            vu_clip_plane(i);
        }
    } 

    // triangle construction
    if (vu_state.m_has_clipped_tri & !vu_state.m_discard) {
        int cp = vu_state.m_vertex_index_size[6];
        if (cp == 0) vu_state.m_discard = true;
        else {
            if (cp > 6) {
                if (vu_state.m_debug)
                    printf("Unexpected!! cp = %d\n",cp);
            }

            if (cp < 3) { 
                if (vu_state.m_debug)
                    printf("Not enough clipped vertices !! cp = %d\n",cp);
            }

            vu_state.s_tri_c[0].v[0] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][0]].v;
            vu_state.s_tri_c[0].v[1] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][1]].v;
            vu_state.s_tri_c[0].v[2] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][2]].v;
            vu_state.m_num_clipped_tri++;

            if (cp ==4) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.m_num_clipped_tri++;
            } else if (cp == 5) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[2].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.s_tri_c[2].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.s_tri_c[2].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.m_num_clipped_tri++;
            } else if (cp == 6) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[2].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.s_tri_c[2].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][5]].v;
                vu_state.s_tri_c[2].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[3].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.s_tri_c[3].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[3].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
            }
        }
    }
}

bool vu_is_all_vertex_inside() {
    if ((vu_state.m_v_buffer[0].outcode | vu_state.m_v_buffer[1].outcode | vu_state.m_v_buffer[2].outcode) == 0)
        return true; 
    else 
        return false; 
}

bool vu_is_all_vertex_outside() {
    if ((vu_state.m_v_buffer[0].outcode & vu_state.m_v_buffer[1].outcode & vu_state.m_v_buffer[2].outcode) != 0) 
        return true;
    else
        return false; 
}

void vu_gen_outcode(s_clip_vertex *csv, svf_vertex *v) {
    // calculate 6 dot products for each clipping plane
    csv->bc[0] = v->coord.element.fw + v->coord.element.fx;    // X = -1 plane
    csv->bc[1] = v->coord.element.fw - v->coord.element.fx;    // X =  1 plane
    csv->bc[2] = v->coord.element.fw + v->coord.element.fy;    // Y = -1 plane
    csv->bc[3] = v->coord.element.fw - v->coord.element.fy;    // Y =  1 plane
    csv->bc[4] = v->coord.element.fw + v->coord.element.fz;    // Z = -1 plane
    csv->bc[5] = v->coord.element.fw - v->coord.element.fz;    // Z =  1 plane
    //if (vu_state.m_debug) std::cout << "BC[0]= " << csv->bc[0] << std::endl;    
    //if (vu_state.m_debug) std::cout << "BC[1]= " << csv->bc[1] << std::endl;    
    //if (vu_state.m_debug) std::cout << "BC[2]= " << csv->bc[2] << std::endl;    
    //if (vu_state.m_debug) std::cout << "BC[3]= " << csv->bc[3] << std::endl;    
    //if (vu_state.m_debug) std::cout << "BC[4]= " << csv->bc[4] << std::endl;    
    //if (vu_state.m_debug) std::cout << "BC[5]= " << csv->bc[5] << std::endl;    

    // BC < 0: outside
    // BC = 0: on the plane 
    // BC > 0: inside
    // out code
    unsigned int outcode =0;
    int i;
    for (i = 0; i <6; i++) {
        if (i !=0) outcode = outcode << 1;
        if (csv->bc[i] < 0) outcode |= 1;  // set sign flag
    }
    csv->outcode = outcode;
}

void vu_clip_plane(const int plane_no) {
    // plane_no 0: Z =  1 plane (bc[5])
    // plane_no 1: Z = -1 plane (bc[4])
    // plane_no 2: Y =  1 plane (bc[3])
    // plane_no 3: Y = -1 plane (bc[2])
    // plane_no 1: X =  1 plane (bc[1])
    // plane_no 0: X = -1 plane (bc[0])
    int i;
    for (i = 0; i< vu_state.m_vertex_index_size[plane_no]; i++) {
        if (i == 0) vu_clip_move(plane_no,vu_state.m_vertex_index[plane_no][0]); // move: v0
        if (i != (vu_state.m_vertex_index_size[plane_no]-1))
            vu_clip_a_line(plane_no,vu_state.m_vertex_index[plane_no][i],vu_state.m_vertex_index[plane_no][i+1], false);
        else
            vu_clip_a_line(plane_no,vu_state.m_vertex_index[plane_no][i],vu_state.m_vertex_index[plane_no][0], true);
    }
}

void vu_clip_move(const int plane_no, const int idx) {
    unsigned int mask = 1 << plane_no;
    unsigned int outcode = vu_state.m_v_buffer[idx].outcode;
    outcode &= mask;
    if (outcode == 0) {// this vertex is inside
        vu_state.m_vertex_index[plane_no+1][vu_state.m_vertex_index_size[plane_no+1]] = idx;
        vu_state.m_vertex_index_size[plane_no+1]++;
    }
}

void vu_clip_a_line(const int plane_no, const int idx_v0, const int idx_v1, const bool last_v) {
    float a;
    s_clip_vertex *p_csv;
    svf_vertex v;
    unsigned int outcode0 = vu_state.m_v_buffer[idx_v0].outcode;
    unsigned int outcode1 = vu_state.m_v_buffer[idx_v1].outcode;
    unsigned int mask = 1 << plane_no;
    outcode0 &= mask;
    outcode1 &= mask;

    if ((outcode0 & outcode1) == 0) {  // at least one is inside
        if ((outcode0 | outcode1) == 0) { // both inside
            if (!last_v) {
                vu_state.m_vertex_index[plane_no+1][vu_state.m_vertex_index_size[plane_no+1]] = idx_v1;
                vu_state.m_vertex_index_size[plane_no+1]++;
            }
        } else {
            // gen new point
            a = vu_state.m_v_buffer[idx_v0].bc[5-plane_no]/
                (vu_state.m_v_buffer[idx_v0].bc[5-plane_no] - vu_state.m_v_buffer[idx_v1].bc[5-plane_no]);
            vu_state.m_has_clipped_tri = true;
            if (outcode0 !=0) {   // v0 is outside, new p0
                v = vu_interpolate(vu_state.m_v_buffer[idx_v0].v, vu_state.m_v_buffer[idx_v1].v, a);
                p_csv = &(vu_state.m_v_buffer[vu_state.m_v_buffer_size]);
                vu_gen_outcode(p_csv,&v);
                vu_state.m_vertex_index[plane_no+1][vu_state.m_vertex_index_size[plane_no+1]] =
                                                    vu_state.m_v_buffer_size;
                vu_state.m_vertex_index_size[plane_no+1]++;
                vu_state.m_v_buffer[vu_state.m_v_buffer_size].v = v;
                vu_state.m_v_buffer_size++;
            }
            if (outcode1 !=0) {   // v1 is outside, new p1
                v = vu_interpolate(vu_state.m_v_buffer[idx_v0].v, vu_state.m_v_buffer[idx_v1].v, a);
                p_csv = &(vu_state.m_v_buffer[vu_state.m_v_buffer_size]);
                vu_gen_outcode(p_csv, &v);
                vu_state.m_vertex_index[plane_no+1][vu_state.m_vertex_index_size[plane_no+1]] =
                                                    vu_state.m_v_buffer_size;
                vu_state.m_vertex_index_size[plane_no+1]++;
                vu_state.m_v_buffer[vu_state.m_v_buffer_size].v = v;
                vu_state.m_v_buffer_size++;
            } else {
                // set original v1
                if (!last_v) {
                    vu_state.m_vertex_index[plane_no+1][vu_state.m_vertex_index_size[plane_no+1]] = idx_v1;
                    vu_state.m_vertex_index_size[plane_no+1]++;
                }
            }
        }
    }  // both are outside
}


void vu_set_blend_enable(bool b) {
    set_3d_reg(PP_BLEND_OPERATION_DEF, b);
}

void vu_clear_color_buffer_all() {
    int i;
    unsigned int x;
    unsigned int top_adrs;

    for (i = 0; i<4; i++) {
		if (i==0) top_adrs = FB00_ADRS;
		if (i==1) top_adrs = FB01_ADRS;
		if (i==2) top_adrs = FB10_ADRS;
		if (i==3) top_adrs = FB11_ADRS;
        for (x = top_adrs; x < top_adrs + 0x96000; x += 4 ) {
            (*(volatile unsigned int  *)x) = 0;
        }
	}
}

void vu_clear_color_buffer(bool ms_flag, float *c) {
    // currently only support 5:6:5
    unsigned int x;
    unsigned int  r,g,b, cv, cvl;
    unsigned int top_adrs;
    r = *(c) * (float)0x1f;
    if (r > 0x1f) r = 0x1f;
    g = *(c+1) * (float)0x3f;
    if (g > 0x3f) g = 0x3f;
    b = *(c+2) * (float)0x1f;
    if (b > 0x1f) b = 0x1f;
    cv = b;
    cv |= (g << 5);
    cv |= (r << 11);
    cvl = cv;
    cvl |= (cv << 16);
    x = PP_FRONT_BUFFER;
    x &= 1;
    if (x) {
        top_adrs = (ms_flag) ? FB01_ADRS : FB00_ADRS;
    }else {
        top_adrs = (ms_flag) ? FB11_ADRS : FB10_ADRS;
    }

    // DMA configuration
    x = PP_DMA_CTRL;
    if (!ms_flag) {
        PP_DMA_TOP_ADRS0 = top_adrs;
        x |= 0x10;
    } else {
        PP_DMA_TOP_ADRS1 = top_adrs;
        x |= 0x20;
    }
    PP_DMA_BE_LENGTH = 0x0f025800;
    PP_DMA_WD0 = cvl;
    PP_DMA_CTRL = x;  // set DMA mode
    //while (!dma_end) ; // wait for end int
}

void vu_clear_depth_buffer(bool ms_flag, float c) {
    unsigned int x;
    unsigned int dv, dvl;
 
    dv = c * (float)0xffff;
    if (dv > 0xffff) dv = 0xffff;
    dvl = dv;
    dvl |= (dv << 16);

    // DMA configuration
    x = PP_DMA_CTRL;
    if (!ms_flag) {
        PP_DMA_TOP_ADRS2 = FB20_ADRS;
        x |= 0x40;
    } else {
        PP_DMA_TOP_ADRS3 = FB21_ADRS;
        x |= 0x80;
    }
    PP_DMA_BE_LENGTH = 0x0f025800;
    PP_DMA_WD1 = dvl;
    PP_DMA_CTRL = x;  // set DMA mode
}

void vu_clear_start() {
    unsigned int x;
    dma_end = 0;
    x = PP_DMA_CTRL;
    x |= 1;
    PP_DMA_CTRL = x;  // start
}

void vu_set_texture(int width, int height, unsigned int adrs,
                    e_texture_type format,  e_texture_pixel_data_type type, void *p) {
    float fw,fh;
    int j;
    unsigned int i,d;
    unsigned char *p_texel;
    unsigned int *p_texel_ui;
    fw = (float)width  - 1.0;
    fh = (float)height - 1.0;

    set_3d_reg(PP_TEX_OFFSET_DEF,adrs);
    set_3d_reg(PP_TEX_WIDTH_M1_DEF, vu_ftoui(fw));
    set_3d_reg(PP_TEX_HEIGHT_M1_DEF, vu_ftoui(fh));
    set_3d_reg(PP_TEX_WIDTH_UI_DEF, width);
    set_3d_reg(PP_TEX_CONFIG_DEF, 0x300);  // 8888

    if (type == UNSIGNED_BYTE) p_texel = (unsigned char*)p;
    else p_texel_ui = (unsigned int*)p;
    for (i = 0; i < width*height; i++) {
        if (type == UNSIGNED_BYTE) {
            if (format == R8G8B8) {
                 for (j=0;j<3;j++) {
                     if (j ==0) d = 0;
                      d |= (*(p_texel++) & 0xff);
                      d = d << 8;
                 }
                 d |= 0xff;
            } else {
                 //R8G8B8A8
                 for (j=0;j<4;j++) {
                     if (j ==0) d = 0;
                      d |= (*(p_texel++) & 0xff);
                      if (j != 3) d = d << 8;
                 }
            }
            (*(volatile unsigned int  *)(adrs+i*4)) =d; 
        } else {
            // UNSIGNED_INT_8_8_8_8
            (*(volatile unsigned int  *)(adrs+i*4)) = *(p_texel_ui+i); 
        }
    }
};

void vu_set_texture_etc(int width, int height, unsigned int adrs, int size, unsigned int *p) {
    float fw,fh;
    unsigned int i;
    fw = (float)width  - 1.0;
    fh = (float)height - 1.0;

    //i = 0x400;   // ETC format
    set_3d_reg(PP_TEX_OFFSET_DEF,adrs);
    set_3d_reg(PP_TEX_WIDTH_M1_DEF, vu_ftoui(fw));
    set_3d_reg(PP_TEX_HEIGHT_M1_DEF, vu_ftoui(fh));
    set_3d_reg(PP_TEX_WIDTH_UI_DEF, width);
    set_3d_reg(PP_TEX_CONFIG_DEF, 0x400);  // ETC format

    for (i = 0; i < size; i++) {
        (*(volatile unsigned int  *)(adrs+i*4)) = *(p+i); 
    }
};

void vu_texture_blender_en(bool en) {
    set_3d_reg(PP_TEX_BLEND_CTRL_DEF,en);
}


void vu_set_multisample(bool en) {
    unsigned int x;
    x = PP_VIDEO_START;
    if (en) x |= 0x300;
    else x &= 0xff;
    PP_VIDEO_START = x;
    PP_RASTER_START = 0x100;
}



void vu_draw_array(int first, int count) {
    int i,j,k;
    vu_state.m_draw_array_first = first;
    vu_state.m_draw_array_cnt = count;
    int num_proc_loop = vu_state.m_draw_array_cnt / PP_VU_DRAW_ARRAY_SIZE;
    int remain = vu_state.m_draw_array_cnt % PP_VU_DRAW_ARRAY_SIZE;
    if (remain != 0) num_proc_loop++;
    int array_size_of_block;
    vu_matrix_prep();
    for (i = 0; i < num_proc_loop; i++) {   // block loop
        if ( i == num_proc_loop -1) {
            array_size_of_block = (remain != 0) ? remain : PP_VU_DRAW_ARRAY_SIZE; 
        } else array_size_of_block = PP_VU_DRAW_ARRAY_SIZE;
        vu_apply_modelview_projection_matrix_da(i,array_size_of_block);
        for (j = 0; j < array_size_of_block/3; j++) {
            vu_clipper_poly_draw_array(i,j);
            if (!vu_state.m_discard) {
                vu_apply_perspective_division();
                vu_apply_view_transform();
                if (!vu_state.m_has_clipped_tri) {
                    vu_swap_vertex(0, &(vu_state.s_tri));
                    if (!vu_state.m_discard) { // bottom.y 
                        if (vu_cull_face(&(vu_state.s_tri))) {
                            if (vu_state.m_lighting_enable) vu_apply_lighting_tri(0);            
                            vu_triangle_output(0);
                        }
                    }
                } else {
                     for (k =0; k < vu_state.m_num_clipped_tri; k++) {
                         vu_swap_vertex(k, &(vu_state.s_tri_c[k]));
                         if (!vu_state.m_discard) { // bottom.y 
                             if (vu_cull_face(&(vu_state.s_tri_c[k]))) { 
                                  if (vu_state.m_lighting_enable) vu_apply_lighting_tri(k);
                                  vu_triangle_output(k);
                             }
                         }
                     }
                }
            }
        }  // for (j

    }  // for (i
}


void vu_apply_modelview_projection_matrix_da(int block_no, int array_size) {
    float *p;
    float *pi;
	float *pn;
    float *pni;
    float *p_weight, *p_weight_i;
    unsigned char   *p_matrix_index, *p_matrix_index_i;
    int i;

    p = vu_state.p_vertex_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_vertex_array_size ;
	if (vu_state.m_matrix_palette_en) {
        p_weight = vu_state.p_weight_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_weight_array_size ;
        p_matrix_index = vu_state.p_matrix_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_matrix_array_size ;
        if (vu_state.m_lighting_enable) {
            pn = vu_state.p_normal_array + block_no*PP_VU_DRAW_ARRAY_SIZE*3 ;
		}
    }
    for (i = 0; i < array_size; i++) {  // x,y,z
        pi = p + vu_state.m_vertex_array_size*i;
        if (vu_state.m_matrix_palette_en) {
            p_weight_i = p_weight + vu_state.m_weight_array_size*i ;
            p_matrix_index_i = p_matrix_index + vu_state.m_matrix_array_size*i ;
            if (vu_state.m_lighting_enable) {
               pni = pn + 3*i ;
 			}
        }

        if (vu_state.m_matrix_palette_en) {
			vu_apply_matrix_palette_f2((float*)vu_state.m_da_buffer[i].coord.f,pi, p_weight_i, p_matrix_index_i);
            if (vu_state.m_lighting_enable) {
				vu_apply_matrix_palette_f2_n((float*)vu_state.m_da_buffer[i].n.f,pni, p_weight_i, p_matrix_index_i);
			}
		} else {
            vu_apply_modelview_projection_matrix_f2((float*)vu_state.m_da_buffer[i].coord.f,pi);
		}

        if (vu_state.m_lighting_enable) {
            if (vu_state.m_matrix_palette_en) {
                vu_state.m_da_buffer[i].coord_eye.element.fx = vu_state.m_da_buffer[i].coord.element.fx;
                vu_state.m_da_buffer[i].coord_eye.element.fy = vu_state.m_da_buffer[i].coord.element.fy;
                vu_state.m_da_buffer[i].coord_eye.element.fz = vu_state.m_da_buffer[i].coord.element.fz;
                vu_state.m_da_buffer[i].coord_eye.element.fw = vu_state.m_da_buffer[i].coord.element.fw;
			} else {
			    vu_state.m_da_buffer[i].coord_eye.element.fx = *(pi);
                vu_state.m_da_buffer[i].coord_eye.element.fy = *(pi+1);
                vu_state.m_da_buffer[i].coord_eye.element.fz = *(pi+2);
                if (vu_state.m_vertex_array_size > 3)
                    vu_state.m_da_buffer[i].coord_eye.element.fw =*(pi+3);
                else
                    vu_state.m_da_buffer[i].coord_eye.element.fw =1.0;
			}
        }
        if (vu_state.m_matrix_palette_en) {
            vu_apply_projection_matrix_f((float*)vu_state.m_da_buffer[i].coord.f);
		}

	    if (vu_state.m_debug) {
            printf("%d %x ",i,p);
            printf("original coordinates = %f %f %f %f\n",
                    vu_state.m_da_buffer[i].coord_eye.element.fx,
                    vu_state.m_da_buffer[i].coord_eye.element.fy,
                    vu_state.m_da_buffer[i].coord_eye.element.fz,
                    vu_state.m_da_buffer[i].coord_eye.element.fw);
        }
	    if (vu_state.m_debug) {
            printf("%d ",i);
            printf("clip coordinates = %f %f %f %f\n",
            		vu_state.m_da_buffer[i].coord.f[0],
            		vu_state.m_da_buffer[i].coord.f[1],
            		vu_state.m_da_buffer[i].coord.f[2],
            		vu_state.m_da_buffer[i].coord.f[3]);
        }
    }

}


void vu_clipper_poly_draw_array(int block_no, int tri_no) {
    int i,j;
    vu_state.m_has_clipped_tri = false;
    vu_state.m_discard = false;
    vu_state.m_num_clipped_tri = 0;
    float *p, *pn, *pt;
    float *pi, *pni, *pti;

    // vector initializaiton
    vu_state.m_v_buffer_size = 0;
    for (i = 0; i<7; i++)
        vu_state.m_vertex_index_size[i] = 0;

    // calculate outcodes for original triangles
    s_clip_vertex *p_csv;
    for (i=0; i<3; i++) {
        //vu_state.s_tri.v[i] = vu_state.m_da_buffer[tri_no*3+i]; // copy triangle vertices
        for (j = 0; j <4; j++) {
            vu_state.s_tri.v[i].coord.f[j] = vu_state.m_da_buffer[tri_no*3+i].coord.f[j];
            if (vu_state.m_lighting_enable)
                vu_state.s_tri.v[i].coord_eye.f[j] = vu_state.m_da_buffer[tri_no*3+i].coord_eye.f[j];
        }
        p_csv = &(vu_state.m_v_buffer[i]);
        vu_gen_outcode(p_csv,&(vu_state.s_tri.v[i]));
        vu_state.m_vertex_index[0][i] = vu_state.m_v_buffer_size;
        vu_state.m_v_buffer_size++;
        vu_state.m_vertex_index_size[0]++;
    }

    if (vu_is_all_vertex_inside()) {
        vu_state.m_has_clipped_tri = false;
        // set parameters
        if (vu_state.m_color_array_en)  p = vu_state.p_color_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_color_array_size;
        if (vu_state.m_normal_array_en) pn = vu_state.p_normal_array + block_no*PP_VU_DRAW_ARRAY_SIZE*3;
        if (vu_state.m_tex_array_en)    pt = vu_state.p_tex_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_tex_array_size;
        for (i = 0; i<3; i++) {  // per vertex
            if (vu_state.m_color_array_en) {
                pi = p + tri_no*3*vu_state.m_color_array_size + vu_state.m_color_array_size*i;
                vu_state.s_tri.v[i].c.r = *(pi);
                vu_state.s_tri.v[i].c.g = *(pi+1);
                vu_state.s_tri.v[i].c.b = *(pi+2);
                if (vu_state.m_color_array_size > 3) 
                    vu_state.s_tri.v[i].c.a = *(pi+3);
            }
            if (vu_state.m_normal_array_en) {            
				if (!vu_state.m_matrix_palette_en) {
                    pni = pn + tri_no*3*3 + 3*i;
                    vu_state.s_tri.v[i].n.element.fx = *(pni);
                    vu_state.s_tri.v[i].n.element.fy = *(pni+1);
                    vu_state.s_tri.v[i].n.element.fz = *(pni+2);
                    vu_state.s_tri.v[i].n.element.fw = 0.0;
				} else {
                    vu_state.s_tri.v[i].n.element.fx = vu_state.m_da_buffer[tri_no*3+i].n.element.fx;
                    vu_state.s_tri.v[i].n.element.fy = vu_state.m_da_buffer[tri_no*3+i].n.element.fy;
                    vu_state.s_tri.v[i].n.element.fz = vu_state.m_da_buffer[tri_no*3+i].n.element.fz;
                    vu_state.s_tri.v[i].n.element.fw = 0.0;
				}
            }
            if (vu_state.m_tex_array_en) {
                pti = pt + tri_no*3*vu_state.m_tex_array_size + vu_state.m_tex_array_size*i;
                vu_state.s_tri.v[i].t.u = *(pti);
                vu_state.s_tri.v[i].t.v = *(pti+1);
            }
        }
    } else if (vu_is_all_vertex_outside()) {
        vu_state.m_discard = true;   // will be not rendered
    } else {
        // do clipping 
        // here set parameters to m_v_buffer
        if (vu_state.m_color_array_en)  p = vu_state.p_color_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_color_array_size;
        if (vu_state.m_normal_array_en) pn = vu_state.p_normal_array + block_no*PP_VU_DRAW_ARRAY_SIZE*3;
        if (vu_state.m_tex_array_en)    pt = vu_state.p_tex_array + block_no*PP_VU_DRAW_ARRAY_SIZE*vu_state.m_tex_array_size;

        for (i = 0; i<3; i++) {
            for (j = 0; j <4; j++) {
                vu_state.m_v_buffer[i].v.coord.f[j] = vu_state.m_da_buffer[tri_no*3+i].coord.f[j];
                if (vu_state.m_lighting_enable)
                    vu_state.m_v_buffer[i].v.coord_eye.f[j] = vu_state.m_da_buffer[tri_no*3+i].coord_eye.f[j];
            }

            if (vu_state.m_color_array_en) {
                pi = p + tri_no*3*vu_state.m_color_array_size + vu_state.m_color_array_size*i;
                vu_state.m_v_buffer[i].v.c.r = *(pi);
                vu_state.m_v_buffer[i].v.c.g = *(pi+1);
                vu_state.m_v_buffer[i].v.c.b = *(pi+2);
                if (vu_state.m_color_array_size > 3) 
                    vu_state.m_v_buffer[i].v.c.a = *(p + tri_no*3*vu_state.m_color_array_size + vu_state.m_color_array_size*i+3);
            }
            if (vu_state.m_normal_array_en) {
				if (!vu_state.m_matrix_palette_en) {
                    pni = pn + tri_no*3*3 + 3*i;
                    vu_state.m_v_buffer[i].v.n.element.fx = *(pni);
                    vu_state.m_v_buffer[i].v.n.element.fy = *(pni+1);
                    vu_state.m_v_buffer[i].v.n.element.fz = *(pni+2);
                    vu_state.m_v_buffer[i].v.n.element.fw = 0.0;
				} else {
                    vu_state.m_v_buffer[i].v.n.element.fx = vu_state.m_da_buffer[tri_no*3+i].n.element.fx;
                    vu_state.m_v_buffer[i].v.n.element.fy = vu_state.m_da_buffer[tri_no*3+i].n.element.fy;
                    vu_state.m_v_buffer[i].v.n.element.fz = vu_state.m_da_buffer[tri_no*3+i].n.element.fz;
                    vu_state.m_v_buffer[i].v.n.element.fw = 0.0;
				}
            }
            if (vu_state.m_tex_array_en) {
                pti = pt + tri_no*3*vu_state.m_tex_array_size + vu_state.m_tex_array_size*i;
                vu_state.m_v_buffer[i].v.t.u = *(pti);
                vu_state.m_v_buffer[i].v.t.v = *(pti+1);
            }
        }
        for (i = 0; i <6; i++) {  // per clipping plane
            vu_clip_plane(i);
        }
    } 

    // triangle construction
    // this should care polygon direction?
    if (vu_state.m_has_clipped_tri & !vu_state.m_discard) {
        int cp = vu_state.m_vertex_index_size[6];
        if (cp == 0) vu_state.m_discard = true;
        else {
            if (cp > 6) {
                //std::cout << "Unexpected!! cp = " << cp << std::endl;exit(1);
            }

            if (cp < 3) { 
                //std::cout << "Not enough clipped vertices !! " << cp << std::endl;
            }

            vu_state.s_tri_c[0].v[0] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][0]].v;
            vu_state.s_tri_c[0].v[1] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][1]].v;
            vu_state.s_tri_c[0].v[2] =  vu_state.m_v_buffer[ vu_state.m_vertex_index[6][2]].v;
            vu_state.m_num_clipped_tri++;

            if (cp ==4) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.m_num_clipped_tri++;
            } else if (cp == 5) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[2].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.s_tri_c[2].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.s_tri_c[2].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.m_num_clipped_tri++;
            } else if (cp == 6) {
                vu_state.s_tri_c[1].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[1].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][3]].v;
                vu_state.s_tri_c[1].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[2].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.s_tri_c[2].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][5]].v;
                vu_state.s_tri_c[2].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.m_num_clipped_tri++;
                vu_state.s_tri_c[3].v[0] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][0]].v;
                vu_state.s_tri_c[3].v[1] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][2]].v;
                vu_state.s_tri_c[3].v[2] = vu_state.m_v_buffer[vu_state.m_vertex_index[6][4]].v;
                vu_state.m_num_clipped_tri++;
            }
        }
    }
}
