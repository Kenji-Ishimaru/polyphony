//=======================================================================
// Project Polyphony
//
// File:
//   pplib_gl.c
//
// Abstract:
//   GL implementation
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
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GLES/glext2.h>
#include <GL/gl.h>
#include <GL/glu.h>

extern pl_vu_state vu_state;
extern pl_matrix4  cur_pm;    // current projection matrix
extern pl_matrix4  cur_tm;    // current texture matrix
extern pl_matrix4  cur_mm;    // current model matrix
extern pl_matrix4  cur_vm;    // current view matrix
extern pl_matrix4  cur_cm;    // current color matrix

extern pl_matrix4* cur_matrix;  // current matrix stack
extern pl_matrix4  matrix_stack[PP_CONFIG_MAX_MSTACK];
extern int         matrix_stack_ptr;

extern bool has_color;
extern bool has_tex;
extern bool has_normal;
extern bool has_weight;
extern bool has_matrix_index;
extern bool b_gl_color_clear;
extern bool b_gl_depth_clear;
extern bool b_gl_texture_2d;
extern bool b_gl_lighting;
extern bool b_gl_light[PP_CONFIG_MAX_LIGHTS];
extern bool b_gl_multisample;

extern unsigned int attr_pointer;

// current state
extern float current_t[2];  // u, v
extern float current_c[4];  // r, g, b, a
extern float current_n[3];  // nx, ny nz

extern float clear_color[4];
extern float clear_z;

extern e_depth_mode m_depth_mode;
extern bool b_gl_depth_test;

// texture environment
extern unsigned int m_cur_gen_tex;
extern unsigned int m_bind_tex;
extern s_texture_environment s_texenv[PP_MAX_TEXENV];

extern t_fui u_fui;

void glViewport( GLint x, GLint y, GLsizei width, GLsizei height ) {
    vu_state.m_vp_height = height;
    vu_state.m_vp_width = width;
    vu_state.m_vp_y = y;
    vu_state.m_vp_x = x;
}

void glFrustumf( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near_val, GLfloat far_val ) {
    // | a[0]  a[1]  a[2]  a[3] |
    // | a[4]  a[5]  a[6]  a[7] |
    // | a[8]  a[9]  a[10] a[11]|
    // | a[12] a[13] a[14] a[15]|
    int i;
    pl_matrix4 m;
    float a[16];
    float m_20;
    m_20 = 2.0;
    for (i=0; i<16; i++)
        a[i] = 0.0;
    a[0]  = m_20*near_val/(right - left);
    a[2]  = (right + left)/(right - left);
    a[5]  = m_20*near_val/(top -bottom);
    a[6]  = (top + bottom)/(top - bottom);
    a[10] = -(far_val + near_val)/(far_val - near_val);
    a[11] = -m_20*near_val*far_val/(far_val - near_val);
    a[14] = -1.0F;

    pl_matrix4_set0(&m, a);
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &m);
}


void glOrthof( GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near_val, GLfloat far_val ) {
    // | a[0]  a[1]  a[2]  a[3] |
    // | a[4]  a[5]  a[6]  a[7] |
    // | a[8]  a[9]  a[10] a[11]|
    // | a[12] a[13] a[14] a[15]|
    int i;
    pl_matrix4 m;
    float a[16];
    for (i=0; i<16; i++)
        a[i] = 0.0;
    a[0]  = 2.0F/(right - left);
    a[3]  = -(right + left)/(right - left);
    a[5]  = 2.0F/(top -bottom);
    a[7]  = -(top + bottom)/(top - bottom);
    a[10] = -2.0F/(far_val - near_val);
    a[11] = -(near_val+far_val)/(far_val - near_val);
    a[15] = 1.0F;

    pl_matrix4_set0(&m, a);
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &m);
}

void glMatrixMode( GLenum mode ) {
    switch (mode) {
        case GL_MODELVIEW:
            cur_matrix = &cur_mm;
            break;
        case GL_PROJECTION:
            cur_matrix = &cur_pm;
            break;
        case GL_TEXTURE:
            cur_matrix = &cur_tm;
            break;
        case GL_MATRIX_PALETTE_OES:
            cur_matrix = &vu_state.matrix_palette[0]; // actual matrix is set by glCurrentPaletteMatrixOES()
            break;
//        case GL_COLOR:
//            cur_matrix = &cur_cm;
//            break;
        default:
            if (vu_state.m_debug)
                printf("Unexpected matrix mode.\n");
            break;
    }
}

void glCurrentPaletteMatrixOES(GLuint index) {
    cur_matrix = &vu_state.matrix_palette[index]; 
	vu_state.m_cur_palette_matrix = index;
}

void glLoadPaletteFromModelViewMatrixOES() {
	pl_matrix4_set2(&vu_state.matrix_palette[vu_state.m_cur_palette_matrix],&cur_mm);
}

void glLoadIdentity( void ) {
    pl_matrix4_identity(cur_matrix);
}

void glTranslatef( GLfloat x, GLfloat y, GLfloat z ) {
    pl_vertex4 v;
    v.x = x; v.y = y; v.z = z; v.w = 1.0;
    pl_matrix4_multiply_vertex4(&v, cur_matrix, &v);
    cur_matrix->a[3] =  v.x;
    cur_matrix->a[7] =  v.y;
    cur_matrix->a[11] = v.z;
}

void glVertex3f( GLfloat x, GLfloat y, GLfloat z ) {

    if (has_normal) {
        vu_set_normal3_once(current_n);
    }
    if (has_tex) {
        vu_set_texcoord2(current_t);
    }

    if (has_color) {
        vu_set_color4(current_c);
    }

    vu_set_vertex4(x,y,z,1.0);
}

void glColor3f( GLfloat red, GLfloat green, GLfloat blue ) {
    has_color = true;
    vu_state.has_color = true;
    current_c[0] = red;
    current_c[1] = green;
    current_c[2] = blue;
    current_c[3] = 1.0;
}

void glColor4f( GLfloat red, GLfloat green, GLfloat blue,GLfloat alpha ) {
    has_color = true;
    vu_state.has_color = true;
    current_c[0] = red;
    current_c[1] = green;
    current_c[2] = blue;
    current_c[3] = alpha;
}

void glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz ) {
    has_normal = true;
    current_n[0] = nx;
    current_n[1] = ny;
    current_n[2] = nz;
}

void glScalef( GLfloat x, GLfloat y, GLfloat z ) {
    // | a[0]  a[1]  a[2]  a[3] |
    // | a[4]  a[5]  a[6]  a[7] |
    // | a[8]  a[9]  a[10] a[11]|
    // | a[12] a[13] a[14] a[15]|
    pl_matrix4 m;
    pl_matrix4_identity(&m);
    m.a[0] = x;
    m.a[5] = y;
    m.a[10] = z;
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &m);
}

void glRotatef( GLfloat angle, GLfloat x, GLfloat y, GLfloat z ){
    float theta = angle /180.0 * MM_PI;
    pl_vector3 v;
    pl_vector3_set0(&v, x,y,z);
    pl_matrix4 m2;
    pl_matrix4_rotate(&m2, &v, theta);
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &m2);
}


void glBegin( GLenum mode ) {
    unsigned int ua;
    // set attribute
    ua = 0;
    if (has_color) {
        ua = 0x00003001;
    }    
    if (has_tex) {
        ua |= 0x12010000;
    }
        
    if (has_color & has_tex) vu_texture_blender_en(true);  // currently only support multiply color & tex
    else vu_texture_blender_en(false);
    set_3d_reg(PP_VTX_ATTR_DEF, ua);
    attr_pointer = get_current_buffer_ptr();// attribute should be reset in glEnd()
    set_all_matrix();
}

void glEnd( void ) {
    unsigned int ua;
    // set attribute
    ua = 0;
    if (has_color) {
        ua = 0x00003001;
    }    
    if (has_tex) {
        ua |= 0x12010000;
    }
    // reset attribute
    set_3d_reg_ptr(PP_VTX_ATTR_DEF, ua,attr_pointer);
}

void glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha ) {
    clear_color[0] = red;
    clear_color[1] = green;
    clear_color[2] = blue;
    clear_color[3] = alpha;
}

void glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha) {
    unsigned int d;
    d = 0;
    if (red) d |= 0x40;
    if (green) d |= 0x20;
    if (blue) d |= 0x10;
    if (alpha) d |= 0x01;

    set_3d_reg(PP_COLOR_MASK_DEF,d);
}

void glClear( GLbitfield mask ) {
#ifndef DUAL_VTX_BUFFER
    cache_init();
    if (mask & GL_COLOR_BUFFER_BIT) {
        vu_clear_color_buffer(cur_color_bank_assign, clear_color);
        if (b_gl_multisample)
            vu_clear_color_buffer(true, clear_color);
    }
    if (mask & GL_DEPTH_BUFFER_BIT) {
        vu_clear_depth_buffer(false, clear_z);
        if (b_gl_multisample|b_buffer_blend)
        //if (b_gl_multisample)
            vu_clear_depth_buffer(true, clear_z);
    }

    vu_clear_start();
#else
    if (mask & GL_COLOR_BUFFER_BIT) {
        b_gl_color_clear = true;
    }
    if (mask & GL_DEPTH_BUFFER_BIT) {
        b_gl_depth_clear = true;
    }
#endif
}

void glClearDepthf( GLclampf depth ) {
   clear_z = depth;
}

void glDepthFunc( GLenum func ) {
    switch(func) {
        case GL_NEVER:
            m_depth_mode = NEVER;
            break;
        case GL_LESS:
            m_depth_mode = LESS;
            break;
        case GL_EQUAL:
            m_depth_mode = EQUAL;
            break;
        case GL_LEQUAL:
            m_depth_mode = LEQUAL;
            break;
        case GL_GREATER:
            m_depth_mode = GREATER;
            break;
        case GL_NOTEQUAL:
            m_depth_mode = NOTEQUAL;
            break;
        case GL_GEQUAL:
            m_depth_mode = GEQUAL;
            break;
        case GL_ALWAYS:
            m_depth_mode = ALWAYS;
            break;
    }

    unsigned int f;
    f = m_depth_mode;
    set_depth_enable(f, b_gl_depth_test);
}


void glTexParameteri (GLenum target, GLenum pname, GLint param) {
    switch(target) {
        case GL_TEXTURE_2D:
            switch(pname) {
                case GL_TEXTURE_MAG_FILTER:
                    switch (param) {
                        case GL_NEAREST:
                            s_texenv[m_bind_tex].m_tex_mag_filter = POINT_SAMPLING;
                        break;
                        case GL_LINEAR:
                            s_texenv[m_bind_tex].m_tex_mag_filter = BILINEAR;
                        break;
                    }
                break;
                case GL_TEXTURE_MIN_FILTER:
                    switch (param) {
                        case GL_NEAREST:
                            s_texenv[m_bind_tex].m_tex_min_filter = POINT_SAMPLING;
                        break;
                        case GL_LINEAR:
                            s_texenv[m_bind_tex].m_tex_min_filter = BILINEAR;
                        break;
                    }
                break;
            }
        break;
    }

}

void glTexImage2D( GLenum target, GLint level, GLint internalFormat,
                   GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type,
                   const GLvoid *pixels ) {
// target: Must be GL_TEXTURE_2D.
// level:  Level 0 is the base image level. Level n is the nth mipmap reduction image.
// internalFormat: number of color components in the texture. 
// width: Must be 2n + 2 ( border ) for some integer n.
// height: Must be 2m + 2 ( border ) for some integer m.
// border: Must be either 0 or 1.
// format: format of the pixel data. 
// type: data type of the pixel data. 
// pixels: a pointer to the image data in memory.  
    s_texenv[m_bind_tex].width = width;
    s_texenv[m_bind_tex].height = height;
    if (format == GL_RGB)
        s_texenv[m_bind_tex].m_tex_format = R8G8B8;
    else 
        s_texenv[m_bind_tex].m_tex_format = R8G8B8A8;
    if (type == GL_UNSIGNED_BYTE)
        s_texenv[m_bind_tex].m_tex_data_type = UNSIGNED_BYTE;
    else
        s_texenv[m_bind_tex].m_tex_data_type = UNSIGNED_INT_8_8_8_8;
    vu_set_texture(s_texenv[m_bind_tex].width, s_texenv[m_bind_tex].height,
                   s_texenv[m_bind_tex].tex_address, 
                   s_texenv[m_bind_tex].m_tex_format,
				   s_texenv[m_bind_tex].m_tex_data_type,
                   (void*) pixels);
}

void glCompressedTexImage2D( GLenum target, GLint level, GLenum internalformat,
                             GLsizei width, GLsizei height, GLint border, GLsizei imageSize,
                             const GLvoid *data ) {
// iamgeSize: Compressed texture images are treated as an array of imageSize ubytes beginning at address data.
    s_texenv[m_bind_tex].m_tex_format = ETC;
    s_texenv[m_bind_tex].width = width;
    s_texenv[m_bind_tex].height = height;
    vu_set_texture_etc(s_texenv[m_bind_tex].width, s_texenv[m_bind_tex].height,
                       s_texenv[m_bind_tex].tex_address, imageSize, (unsigned int*)data);
}

void glTexCoord2f( GLfloat s, GLfloat t ) {
    has_tex = true;
    vu_state.has_tex = true;
    current_t[0] = s;
    current_t[1] = t;
}


void glGenTextures (GLsizei n, GLuint *textures) {
    int i,j;
    // set texture allocation address to textures
    for (i = 0; i <n; i++) {
        textures[i] = m_cur_gen_tex;
        m_cur_gen_tex++;
        // set default blend
        for (j=0; j<3; j++) {
            s_texenv[i].m_tb_arg[0][j] = TB_ARG_TEXTURE0;
        }
        if (m_cur_gen_tex > PP_MAX_TEXENV) {
            if (vu_state.m_debug)
                printf("Over texenv generation limit\n");
        }
    }
}

void glBindTexture (GLenum target, GLuint texture) {
    float fw,fh;
    m_bind_tex = texture;
    // set address offset
    if (texture > 0) { 
        //s_texenv[texture].tex_address = 
        //         s_texenv[texture-1].tex_address + s_texenv[texture-1].height*s_texenv[texture-1].width;
        // This is a hardware limitation of current implementation.
        s_texenv[texture].tex_address = 
                s_texenv[texture-1].tex_address + 0x100000;
    }
    /*    // blending
    */
    fw = (float)s_texenv[texture].width  - 1.0;
    fh = (float)s_texenv[texture].height - 1.0;


    set_3d_reg(PP_TEX_OFFSET_DEF,s_texenv[texture].tex_address);
    u_fui.f = fw;
    set_3d_reg(PP_TEX_WIDTH_M1_DEF, u_fui.ui);
    u_fui.f = fh;
    set_3d_reg(PP_TEX_HEIGHT_M1_DEF, u_fui.ui);
    set_3d_reg(PP_TEX_WIDTH_UI_DEF,  s_texenv[texture].width);
    if (s_texenv[texture].m_tex_format == ETC)
        set_3d_reg(PP_TEX_CONFIG_DEF,0x400);
    else
        set_3d_reg(PP_TEX_CONFIG_DEF, 0x300);  // 8888

}

void glEnable( GLenum cap ) {
    if (cap == GL_CULL_FACE) {
        vu_state.m_cull_face_enable = true;
    }

    if (cap == GL_TEXTURE_2D) {
        b_gl_texture_2d = true;
        set_tex_enable(b_gl_texture_2d);
    }
    if (cap == GL_LIGHTING) {
        b_gl_lighting = true;
        vu_state.m_lighting_enable = true;
        has_color = true;
        vu_state.has_color = true;
    }

    if (cap == GL_LIGHT0) {
        b_gl_light[0] = true;
        vu_state.m_light_enable[0] = true;
    }
    if (cap == GL_LIGHT1) {
        b_gl_light[1] = true;
        vu_state.m_light_enable[1] = true;
    }
    if (cap == GL_LIGHT2) {
        b_gl_light[2] = true;
        vu_state.m_light_enable[2] = true;
    }
    if (cap == GL_LIGHT3) {
        b_gl_light[3] = true;
        vu_state.m_light_enable[3] = true;
    }

    if (cap == GL_MULTISAMPLE) {
        b_gl_multisample = true;
        vu_set_multisample(true);
    }

    if (cap == GL_DEPTH_TEST) {
        b_gl_depth_test = true;
        unsigned int func;
        func = m_depth_mode;
        set_depth_enable(func, b_gl_depth_test);
    }
    if (cap == GL_BLEND) {
        vu_set_blend_enable(true);
    }
    if (cap == GL_COLOR_MATERIAL) {
        vu_state.m_color_material_en = true;
    }
	if (cap == GL_MATRIX_PALETTE_OES) {
        vu_state.m_matrix_palette_en = true;
	}


}

void glDisable( GLenum cap ) {
    if (cap == GL_CULL_FACE) {
        vu_state.m_cull_face_enable = false;
    }

    if (cap == GL_TEXTURE_2D) {
        vu_state.m_tex_array_en = false;
        has_tex = false;
        vu_state.has_tex = false;
        b_gl_texture_2d = false;
        set_tex_enable(b_gl_texture_2d);
    }
    if (cap == GL_LIGHTING) {
        b_gl_lighting = false;
        vu_state.m_lighting_enable = false;
        has_color = false;
        vu_state.has_color = false;
    }

    if (cap == GL_LIGHT0) {
        b_gl_light[0] = false;
        vu_state.m_light_enable[0] = false;
    }
    if (cap == GL_LIGHT1) {
        b_gl_light[1] = false;
        vu_state.m_light_enable[1] = false;
    }
    if (cap == GL_LIGHT2) {
        b_gl_light[2] = false;
        vu_state.m_light_enable[2] = false;
    }
    if (cap == GL_LIGHT3) {
        b_gl_light[3] = false;
        vu_state.m_light_enable[3] = false;
    }

    if (cap == GL_MULTISAMPLE) {
        b_gl_multisample = true;
        vu_set_multisample(true);
    }

    if (cap == GL_DEPTH_TEST) {
        b_gl_depth_test = false;
        unsigned int func;
        func = m_depth_mode;
        set_depth_enable(func, b_gl_depth_test);
    }
    if (cap == GL_BLEND) {
        vu_set_blend_enable(false);
    }
    if (cap == GL_MULTISAMPLE) {
        b_gl_multisample = false;
        vu_set_multisample(false);
    }
	if (cap == GL_MATRIX_PALETTE_OES) {
       vu_state.m_matrix_palette_en = false;
	}

}

void glShadeModel( GLenum mode ) {
    switch (mode) {
        case GL_FLAT:
            vu_state.m_shade_mode = FLAT;
        break;
        case GL_SMOOTH:
            vu_state.m_shade_mode = SMOOTH;
        break;
#ifdef __glext2_h_
        case GL_COOK_TORRANCE_GOLD:
			vu_gen_roughness(256, 0.25);
			vu_gen_fresnel(RED, 256, 0.721);
			vu_gen_fresnel(GREEN, 256, 0.45);
			vu_gen_fresnel(BLUE, 256, 0.2);
            vu_state.m_shade_mode = COOK_TORRANCE;
        break;
		case GL_COOK_TORRANCE_SILVER:
			vu_gen_roughness(256, 0.30);
			vu_gen_fresnel(RED, 256, 0.75);
			vu_gen_fresnel(GREEN, 256, 0.75);
			vu_gen_fresnel(BLUE, 256, 0.75);
			vu_state.m_shade_mode = COOK_TORRANCE;
		break;
#endif
    }
}

void glLightf( GLenum light, GLenum pname, GLfloat param ) {
}

void glLightfv( GLenum light, GLenum pname, const GLfloat *params ) {
    int lnum;
    pl_vertex4 v;

    switch (light) {
        case GL_LIGHT0:
            lnum = 0;
            break;
        case GL_LIGHT1:
            lnum = 1;
            break;
        case GL_LIGHT2:
            lnum = 2;
            break;
        case GL_LIGHT3:
            lnum = 3;
            break;
        default:
            lnum = 0;
            break;
    }

    switch (pname) {
        case GL_AMBIENT:
            vu_state.m_light_param[lnum].ambient.r = params[0];
            vu_state.m_light_param[lnum].ambient.g = params[1];
            vu_state.m_light_param[lnum].ambient.b = params[2];
            vu_state.m_light_param[lnum].ambient.a = params[3];
            break;
        case GL_DIFFUSE:
            vu_state.m_light_param[lnum].diffuse.r = params[0];
            vu_state.m_light_param[lnum].diffuse.g = params[1];
            vu_state.m_light_param[lnum].diffuse.b = params[2];
            vu_state.m_light_param[lnum].diffuse.a = params[3];
            break;
        case GL_SPECULAR:
            vu_state.m_light_param[lnum].specular.r = params[0];
            vu_state.m_light_param[lnum].specular.g = params[1];
            vu_state.m_light_param[lnum].specular.b = params[2];
            vu_state.m_light_param[lnum].specular.a = params[3];
            break;
        case GL_POSITION:
            v.x = params[0];v.y = params[1];v.z = params[2];v.w = params[3];
            pl_matrix4_multiply_vertex4(&v, cur_matrix, &v);
            vu_state.m_light_param[lnum].position.x = v.x;
            vu_state.m_light_param[lnum].position.y = v.y;
            vu_state.m_light_param[lnum].position.z = v.z;
            vu_state.m_light_param[lnum].position.w = v.w;
            break;
        case GL_SPOT_DIRECTION:
        break;
        case GL_SPOT_EXPONENT:
        break;
        case GL_SPOT_CUTOFF:
        break;
        case GL_CONSTANT_ATTENUATION:
        break;
        case GL_LINEAR_ATTENUATION:
        break;
        case GL_QUADRATIC_ATTENUATION:
        break;
    }
}

void glLightModelf( GLenum pname, GLfloat param ) {
}

void glLightModelfv( GLenum pname, const GLfloat *params ) {
   switch (pname) {
        case GL_LIGHT_MODEL_AMBIENT:
            vu_state.m_scene_ambient.r = params[0];
            vu_state.m_scene_ambient.g = params[1];
            vu_state.m_scene_ambient.b = params[2];
            vu_state.m_scene_ambient.a = params[3];
        break;
   }
}

void glMaterialf( GLenum face, GLenum pname, GLfloat param ) {
    switch(face) {
        case GL_FRONT:
            switch (pname) {
                case GL_SHININESS:
                    vu_state.m_front_material.shininess = param;
                break;
        }
    }

}

void glMaterialfv( GLenum face, GLenum pname, const GLfloat *params ) {
    switch(face) {
        case GL_FRONT:
        case GL_FRONT_AND_BACK:
            switch (pname) {
                case GL_AMBIENT:
                    vu_state.m_front_material.ambient.r = *(params);
                    vu_state.m_front_material.ambient.g = *(params+1);
                    vu_state.m_front_material.ambient.b = *(params+2);
                    vu_state.m_front_material.ambient.a = *(params+3);
                break;
                case GL_DIFFUSE:
                    vu_state.m_front_material.diffuse.r = *(params);
                    vu_state.m_front_material.diffuse.g = *(params+1);
                    vu_state.m_front_material.diffuse.b = *(params+2);
                    vu_state.m_front_material.diffuse.a = *(params+3);
                break;
                case GL_SPECULAR:
                    vu_state.m_front_material.specular.r = *(params);
                    vu_state.m_front_material.specular.g = *(params+1);
                    vu_state.m_front_material.specular.b = *(params+2);
                    vu_state.m_front_material.specular.a = *(params+3);
                break;
                case GL_AMBIENT_AND_DIFFUSE:
                    vu_state.m_front_material.ambient.r = *(params);
                    vu_state.m_front_material.ambient.g = *(params+1);
                    vu_state.m_front_material.ambient.b = *(params+2);
                    vu_state.m_front_material.ambient.a = *(params+3);
                    vu_state.m_front_material.diffuse.r = *(params);
                    vu_state.m_front_material.diffuse.g = *(params+1);
                    vu_state.m_front_material.diffuse.b = *(params+2);
                    vu_state.m_front_material.diffuse.a = *(params+3);
                break;
                case GL_EMISSION:
                    vu_state.m_front_material.emission.r = *(params);
                    vu_state.m_front_material.emission.g = *(params+1);
                    vu_state.m_front_material.emission.b = *(params+2);
                    vu_state.m_front_material.emission.a = *(params+3);
                break;

            }
            break;
        case GL_BACK:
            break;
    }

}

void glColorMaterial( GLenum face, GLenum mode ) {

}

void glEnableClientState (GLenum array) {
    switch (array) {
        case GL_VERTEX_ARRAY:
            vu_state.m_vertex_array_en = true;
            break;
        case GL_NORMAL_ARRAY:
            vu_state.m_normal_array_en = true;
            has_color = true;
            vu_state.has_color = true;
            break;
        case GL_COLOR_ARRAY:
            vu_state.m_color_array_en = true;
            has_color = true;
            vu_state.has_color = true;
            break;
        case GL_TEXTURE_COORD_ARRAY:
            vu_state.m_tex_array_en = true;
            has_tex = true;
            vu_state.has_tex = true;
            break;
		case GL_WEIGHT_ARRAY_OES:
            vu_state.m_weight_array_en = true;
            break;
        case GL_MATRIX_INDEX_ARRAY_OES:
            vu_state.m_matrix_array_en = true;
            break;
    }
}


void glDisableClientState (GLenum array) {
    switch (array) {
        case GL_VERTEX_ARRAY:
            vu_state.m_vertex_array_en = false;
            break;
        case GL_NORMAL_ARRAY:
            vu_state.m_normal_array_en = false;
            if (!vu_state.m_color_array_en) {
                has_color = false;
                vu_state.has_color = false;
            }
            break;
        case GL_COLOR_ARRAY:
            vu_state.m_color_array_en = false;
            if (!vu_state.m_normal_array_en) {
                has_color = false;
                vu_state.has_color = false;
            }
            break;
        case GL_TEXTURE_COORD_ARRAY:
            vu_state.m_tex_array_en = false;
            has_tex = false;
            vu_state.has_tex = false;
            break;
		case GL_WEIGHT_ARRAY_OES:
            vu_state.m_weight_array_en = false;
            break;
        case GL_MATRIX_INDEX_ARRAY_OES:
            vu_state.m_matrix_array_en = false;
            break;
    }
}

void glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.m_color_array_size = size;
    vu_state.p_color_array = (float*)pointer;
 }

void glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.p_normal_array = (float*)pointer;
}

void glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.m_tex_array_size = size;
    vu_state.p_tex_array = (float*)pointer;
}

void glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.m_vertex_array_size = size;
    vu_state.p_vertex_array = (float*)pointer;
}

void glWeightPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.m_weight_array_size = size;
    vu_state.p_weight_array = (float*)pointer;
}

void glMatrixIndexPointerOES(GLint size, GLenum type, GLsizei stride, const GLvoid *pointer) {
    vu_state.m_matrix_array_size = size;
    vu_state.p_matrix_array = (unsigned char*)pointer;
}

void glDrawArrays (GLenum mode, GLint first, GLsizei count) {
	unsigned int ua;
    //timer_start();

    rstat_tmp.num_of_injected_triangles += count;
    set_all_matrix();
    // set attribute
    ua = 0;
    if (has_color) {
        ua = 0x00003001;
    }    
    if (has_tex) {
        ua |= 0x12010000;
    }
        
    if (has_color & has_tex) vu_texture_blender_en(true);  // currently only support multiply color & tex
    else vu_texture_blender_en(false);


    set_3d_reg(PP_VTX_ATTR_DEF, ua);


    vu_draw_array(first,count);
}
// GLU
void gluLookAt (GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ, 
                GLdouble centerX, GLdouble centerY, GLdouble centerZ, 
                GLdouble upX, GLdouble upY, GLdouble upZ) {

    // finally the result is stored in view matrix
    int i;

    float a[16];

    pl_vector3 forward, side, up;
    pl_matrix4 m;

    pl_matrix4_identity(&cur_vm);
    forward.x = (float)(centerX - eyeX);
    forward.y = (float)(centerY - eyeY);
    forward.z = (float)(centerZ - eyeZ);

    up.x = (float)upX;
    up.y = (float)upY;
    up.z = (float)upZ;

    pl_vector3_normalize(&forward);

    /* Side = forward x up */
    //side = side.cross_product(forward, up);
    pl_vector3_cross_product0(&side, &forward, &up);
    pl_vector3_normalize(&side);

    /* Recompute up as: up = side x forward */
    //up = up.cross_product(side, forward);
    pl_vector3_cross_product0(&up, &side, &forward);

    for (i=0; i<16; i++)
        a[i] = 0.0;
    a[0] /*m[0][0]*/ = side.x;
    a[4] /*m[1][0]*/ = side.y;
    a[8] /*m[2][0]*/ = side.z;

    a[1] /*m[0][1]*/ = up.x;
    a[5] /*m[1][1]*/ = up.y;
    a[9] /*m[2][1]*/ = up.z;

    a[2]  /*m[0][2]*/ = -forward.x;
    a[6]  /*m[1][2]*/ = -forward.y;
    a[10] /*m[2][2]*/ = -forward.z;
    a[15] = 1.0;
    pl_matrix4_set0(&m, a);
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &m);
    glTranslatef(-eyeX, -eyeY, -eyeZ);

}

void gluPerspective (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar) {
  float r0, r1, s, c;
    r0 = fovy;
    r1 = MM_PI;
    r0 = r0 * r1;
    r1 = 360.0;
    r0 = r0 / r1;
    s = sin(r0);
    c = cos(r0);
    float t = s/c;  //tan
    float ymax = zNear * t;
    float xmax = ymax * aspect;
    glFrustumf(-xmax, xmax, -ymax, ymax, zNear, zFar);
}

void glCullFace( GLenum mode ) {
    if (mode == GL_BACK) vu_state.m_cf_mode = BACK;
    else if (mode == GL_FRONT) vu_state.m_cf_mode = FRONT;
    else  vu_state.m_cf_mode = FRONT_AND_BACK;
}
void glFrontFace( GLenum mode ) {
    if (mode == GL_CCW) vu_state.m_ff_mode = CCW;
    else vu_state.m_ff_mode = CW;
}

void glPopMatrix (void) {
    if (matrix_stack_ptr > 0) {
        matrix_stack_ptr--;
        *cur_matrix = matrix_stack[matrix_stack_ptr];
    }
}

void glPushMatrix (void) {
    matrix_stack[matrix_stack_ptr] = *cur_matrix;
    matrix_stack_ptr++;
}


void glFinish() {
    render_buffered_triangles();
}

void glFlush() {
    render_data_available = 1;
#ifndef DUAL_VTX_BUFFER
    render_buffered_triangles();
    cache_flush();
#endif
}

void GL_APIENTRY glMultMatrixf (const GLfloat *m) {
    pl_matrix4 mm,mt;
	pl_matrix4_set3(&mm,m);
	pl_matrix4_transpose(&mt,&mm);
    pl_matrix4_multiply_matrix4i(cur_matrix, cur_matrix, &mt);
}
