//=======================================================================
// Project Polyphony
//
// File:
//   app_cook_torrance.c
//
// Abstract:
//   anaglyph demo
//
//  Created:
//    31 Oct. 2009
//
// Copyright (c) 2009  Kenji Ishimaru, All rights reserved.
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
			
#include <stdio.h>
#include <math.h>
#include <GLES/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "hwdep.h"

#define WITH_OPENING_SCENE
//#define VERTICAL_FLIP

#define ROT_X_VALUE 75.0

void land_ag();

#ifdef WITH_OPENING_SCENE
void opening_scene_white();

#include "./data/boot_tex.txt"
unsigned int tex_num[2];
#endif

#include "./data/land/xfile_vtx.txt"
#include "./data/land/xfile_nml.txt"

int main(void)
{
   system_init();
   land_ag();

   return 0;
}


float tr_x, tr_y, tr_z;
float step, speed,rot_x;
int   stop_flag;

static char my_getchar() {
  u8 c;
  c = XUartPs_ReadReg(XPS_UART1_BASEADDR,
				      XUARTPS_FIFO_OFFSET);
  return c;
}

void get_input_serial() {
    char c;
    c = my_getchar();
    if (c == '9') glEnable(GL_MULTISAMPLE);
    if (c == '0') glDisable(GL_MULTISAMPLE);
    if (c == 'a') tr_x -= step;
    if (c == 'd') tr_x += step;
    if (c == 'w') tr_y += step;
    if (c == 'x') tr_y -= step;
    if (c == 'r') tr_z -= step;
    if (c == 'v') tr_z += step;
    if (c == 'f') speed += 1.0;
    if (c == 's') {
        tr_x = 0;
        tr_y = 0;
        tr_z = -1;
        speed = 1.0;
    }
    if (c == '1') {
		if (stop_flag == 0) stop_flag = 1;
		else stop_flag = 0;
	}
}

void get_input() {
    get_input_serial();
}


void set_color_buffer_address(int bank) {
    cur_color_bank_assign = bank;   // for color buffer clear
    set_3d_reg(PP_SCREEN_MODE_DEF, ((bank << 8)|1));
}

void gluPerspectiveLeft(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, GLfloat fo, GLfloat eyesep) {
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
    float d = 0.5 * eyesep * zNear/fo;
    float xmax = ymax * aspect;
    glFrustumf(-xmax+d, xmax+d, -ymax, ymax, zNear, zFar);
}

void gluPerspectiveRight(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar, GLfloat fo, GLfloat eyesep){
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
    float d = 0.5 * eyesep * zNear/fo;
    float xmax = ymax * aspect;
    glFrustumf(-xmax-d, xmax-d, -ymax, ymax, zNear, zFar);
}

 GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 0.0};
 GLfloat light0_position[] = {1.0, 1.0, 1.0, 0.0};

void land_ag() {
    int i;
    float eye_separation;
    float near_plane, far_plane, focal_length;
    near_plane = 1.0;
    far_plane = 100.0;
    focal_length = 5.0;
    tr_x = 0;
    tr_y = 0;
    tr_z = -1;
    step = 0.1;
	rot_x = ROT_X_VALUE;
	stop_flag = 0;
    eye_separation = focal_length/30.0;
    gl_init();
    color_buffer_all_clear();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepthf(1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
#ifdef WITH_OPENING_SCENE
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, (unsigned int*)&tex_num);
    // texture0
    glBindTexture(GL_TEXTURE_2D,tex_num[0]);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480,
                           0, 38406-6, boot_texel_array);

    opening_scene_white();
    glDisable(GL_TEXTURE_2D);
#endif
    set_buffer_blend(true);
    // Lighting configuration
    glEnable(GL_LIGHTING);
    // Light
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    // Material
    GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    glVertexPointer(3, GL_FLOAT, 0, vtx_array);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer( GL_FLOAT, 0, nml_array);
    glEnableClientState(GL_NORMAL_ARRAY);

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glViewport(0, 0, 640, 480);
	i = 0;
    while (1) {
        printf("scene = %d\n", i);
        get_input();

        // Left Eye
        set_color_buffer_address(0);
        glColorMask(true, true,true,true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspectiveLeft(45.0,4.0/3.0, near_plane, far_plane, focal_length, eye_separation);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(-eye_separation/2,0,1, -eye_separation/2, 0, 0, 0,1,0);

#ifdef VERTICAL_FLIP
        glRotatef(-90, 0,0,1);
#endif
        glTranslatef(tr_x, tr_y,tr_z);
        glRotatef(rot_x, 1,0,0);
        glRotatef((float)i, 0,1,0);
        glScalef(100.0, 100.0, 100.0);
		glColorMask(true, false,false,true);
        glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
#ifndef DUAL_VTX_BUFFER
        glFlush();
#endif

         // Right eye
        set_color_buffer_address(1);
        glColorMask(true, true,true,true);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspectiveRight(45.0,4.0/3.0, near_plane, far_plane, focal_length, eye_separation);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glColorMask(false, true,true,true);
        gluLookAt(eye_separation/2,0, 1, eye_separation/2, 0, 0, 0,1,0);
#ifdef VERTICAL_FLIP
        glRotatef(-90, 0,0,1);
#endif
        glTranslatef(tr_x, tr_y,tr_z);
        glRotatef(rot_x, 1,0,0);
        glRotatef((float)i, 0,1,0);
        glScalef(100.0, 100.0, 100.0);
		glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
        glFlush();
        wait_vsync();
		if (stop_flag ==0) {
	        i++;
			if (i >= 360) i = 0;
   	    }
   } // while
}
#ifdef WITH_OPENING_SCENE

void opening_scene_white() {
    int i;
    char buf[64];
    float alpha;
    float w,h;
    int  num_fade_frames = 30;
    w = 640; h = 480;
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
	glOrthof(0,640,0,480,-1,1);							// Set Up An Ortho Screen
	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPushMatrix();										// Store The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
    glBindTexture(GL_TEXTURE_2D,tex_num[0]);
    glEnable(GL_BLEND);
    for (i = 0; i <= num_fade_frames*2; i++) {
        if (i < num_fade_frames) {
            // fade in, alpha is 0.0 to 1.0
            alpha = 1.0 * (float)i/(float)(num_fade_frames-1);
        } else {
            // fade out, alpha is 1.0 to 0.0
            alpha = 1.0 * (1.0- (float)(i-num_fade_frames)/(float)(num_fade_frames-1));
        }
        sprintf(buf,"scene %d alpha = %f\n",i,alpha);
        puts(buf);
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBegin(GL_TRIANGLES);
            glColor4f(1.0, 1.0, 1.0,alpha);  // r
            glTexCoord2f(1.0, 0.0);
            glVertex3f( w,  h,-0.5);  // triangle0
            glColor4f(1.0, 1.0, 1.0,alpha);  // g
            glTexCoord2f(0.0, 0.0);
            glVertex3f(0.0,  h,-0.5);
            glColor4f(1.0, 1.0, 1.0,alpha);  // b
            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, 0.0,-0.5);

            glColor4f(1.0, 1.0, 1.0,alpha);  // r
            glTexCoord2f(1.0, 0.0);
            glVertex3f( w,  h,-0.5);  // triangle1
            glColor4f(1.0, 1.0, 1.0,alpha);  // b
            glTexCoord2f(0.0, 1.0);
            glVertex3f(0.0, 0.0,-0.5);
            glColor4f(1.0, 1.0, 1.0,alpha);  // white
            glTexCoord2f(1.0, 1.0);
            glVertex3f( w, 0.0,-0.5);
		glEnd();
        glFlush();
        wait_vsync();
    }
    glDisable(GL_BLEND);
	glPopMatrix();										// Restore The Old Projection Matrix
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPopMatrix();										// Restore The Old Projection Matrix

	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

}

#endif




