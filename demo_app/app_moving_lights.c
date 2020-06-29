//=======================================================================
// Project Polyphony
//
// File:
//   app_moving_lights.c
//
// Abstract:
//   matrix push/pop demo
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

#define WITH_OPENING_SCENE
//#define VERTICAL_FLIP
#include <stdio.h>
#include <GLES/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "hwdep.h"

void moving_lights();

#ifdef WITH_OPENING_SCENE
void opening_scene_white();

#include "./data/boot_tex.txt"
unsigned int tex_num[2];
#endif

float triangle[] = {
    // front      
    0.25,  0.25,0.25,  // triangle0
   -0.25,  0.25,0.25,
   -0.25, -0.25,0.25,
    0.25,  0.25,0.25,  // triangle1
   -0.25, -0.25,0.25,
    0.25, -0.25,0.25,
    // top      
    0.25,  0.25,-0.25, // triangle0
   -0.25,  0.25,-0.25,
   -0.25,  0.25,0.25,
    0.25,  0.25,-0.25, // triangle1
   -0.25,  0.25,0.25,
    0.25,  0.25,0.25,
    // bottom      
    0.25,  -0.25,0.25,  // triangle0
   -0.25,  -0.25,0.25,
   -0.25,  -0.25,-0.25,
    0.25,  -0.25,0.25,  // triangle1
   -0.25,  -0.25,-0.25,
    0.25,  -0.25,-0.25,
    // left      
   -0.25, 0.25,0.25,  // triangle0
   -0.25, 0.25,-0.25,
   -0.25, -0.25,-0.25,
   -0.25, 0.25,0.25,  // triangle1
   -0.25, -0.25,-0.25,
   -0.25, -0.25,0.25,
   // right      
    0.25, 0.25,-0.25,  // triangle0
    0.25, 0.25,0.25,
    0.25, -0.25,0.25,
    0.25, 0.25,-0.25,  // triangle1
    0.25, -0.25,0.25,
    0.25, -0.25,-0.25,
    // back    
   -0.25, 0.25,-0.25,  // triangle0
    0.25, 0.25,-0.25,
    0.25, -0.25,-0.25,
   -0.25, 0.25,-0.25,  // triangle1
    0.25, -0.25,-0.25,
   -0.25, -0.25,-0.25
};

float colors[] = {
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,

    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0,
    1.0, 1.0, 1.0
};

float normals[] = {
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,
    0.0, 0.0, 1.0,

    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,
    0.0, 1.0, 0.0,

    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,
    0.0, -1.0, 0.0,

   -1.0, 0.0, 0.0,
   -1.0, 0.0, 0.0,
   -1.0, 0.0, 0.0,
   -1.0, 0.0, 0.0,
   -1.0, 0.0, 0.0,
   -1.0, 0.0, 0.0,

    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,
    1.0, 0.0, 0.0,

    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0,
    0.0, 0.0, -1.0
};

float texcoords[] = {
    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0,

    1.0, 1.0,
    0.0, 1.0,
    0.0, 0.0,
    1.0, 1.0,
    0.0, 0.0,
    1.0, 0.0
};
//
#include "./data/sphere/sphere_vtx.txt"
#include "./data/sphere/sphere_nml.txt"

float tr_x, tr_y, tr_z;
float step, speed;
int   num_lights, stop_flag;

int main(void)
{
   system_init();
   moving_lights();

   return 0;
}

static char my_getchar() {
  u8 c;
  c = XUartPs_ReadReg(XPS_UART1_BASEADDR,
				      XUARTPS_FIFO_OFFSET);
  return c;
}

void get_input_serial() {
    char c;
    c = my_getchar();;
    if (c == '9') glEnable(GL_MULTISAMPLE);
    if (c == '0') glDisable(GL_MULTISAMPLE);
#ifdef VERTICAL_FLIP
    if (joypad_status.BYTE.y == 0)    tr_x += step;
    if (joypad_status.BYTE.y == 0xff) tr_x -= step;
    if (joypad_status.BYTE.x == 0)    tr_y += step;
    if (joypad_status.BYTE.x == 0xff) tr_y -= step;
#else
    if (c == '1')    tr_x -= step;
    if (c == '2') tr_x += step;
    if (c == '3')    tr_y += step;
    if (c == '4') tr_y -= step;
#endif
    if (c == '5') tr_z -= step;
    if (c == '6') tr_z += step;
    if (c == '7') {
		if (stop_flag == 0) stop_flag = 1;
		else stop_flag = 0;
	}
	if (c == '8') {
        tr_x = 0.65;
        tr_y = -0.65;
        tr_z = -7.0;
    }
    if (c == 'q') {
        num_lights = 1;
    }
    if (c == 'w') {
        num_lights = 2;
    }
    if (c == 'e') {
        num_lights = 3;
    }
    if (c == 's') {
		if (stop_flag == 0) stop_flag = 1;
		else stop_flag = 0;
	}
}


void get_input() {
    get_input_serial();
}

void moving_lights() {
   int i=0,j,k,l,m;
    num_lights = 3;
    stop_flag = 0;

    gl_init();
    glEnable(GL_MULTISAMPLE);
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

    // Lighting configuration
    glEnable(GL_LIGHTING);
    // Light
    glEnable(GL_LIGHT0);
    GLfloat light0_diffuse[] = {1.0, 0.0, 0.0, 0.0};
    GLfloat light0_position[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    //glEnable(GL_LIGHT1);
    GLfloat light1_diffuse[] = {0.0, 1.0, 0.0, 0.0};
    GLfloat light1_position[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    //glEnable(GL_LIGHT2);
    GLfloat light2_diffuse[] = {0.0, 0.0, 1.0, 0.0};
    GLfloat light2_position[] = {0.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_diffuse);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    // Material
    GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 100.0);

    glVertexPointer(3, GL_FLOAT, 0, triangle);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);
    glEnableClientState(GL_COLOR_ARRAY);
    glNormalPointer(GL_FLOAT, 0, normals);
    glEnableClientState(GL_NORMAL_ARRAY);

    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(30.0,4.0/3.0, 1, 100);
    tr_x = 0.65;
    tr_y = -0.65;
    tr_z = -7.0;
    step = 0.05;
    speed = 4.0;
	i = 0;
    while (1) {
        printf("scene = %d\n", i);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,0,1, 0, 0, 0, 0,1,0);
        get_input();
        glTranslatef(tr_x, tr_y,tr_z);    // origin

        glVertexPointer(3, GL_FLOAT, 0, sphere_vtx_array);
        glNormalPointer(GL_FLOAT, 0, sphere_nml_array);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
/*
        glVertexPointer(3, GL_FLOAT, 0, triangle);
        glEnableClientState(GL_VERTEX_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glEnableClientState(GL_COLOR_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals);
        glEnableClientState(GL_NORMAL_ARRAY);
*/
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        if (num_lights > 1) {
            glEnable(GL_LIGHT1);
        }
        if (num_lights > 2) {
            glEnable(GL_LIGHT2);
        }

        // Light 0
        glPushMatrix();
        glRotatef((float)i*3, 0,1,0);
        glTranslatef(2.5, 0,0);
        //glScalef(0.5,0.5,0.5);
        glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
        mat_diffuse[0] = 1.0;mat_diffuse[1] = 0.0;mat_diffuse[2] = 0.0;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_diffuse);
        mat_diffuse[0] = 0.0;mat_diffuse[1] = 0.0;mat_diffuse[2] = 0.0;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
        glPopMatrix();
        if (num_lights > 1) {
        // Light 1
        glPushMatrix();
        glRotatef((float)i*3, 1,0,0);
        glTranslatef(0, 0,2.5);
        //glScalef(0.5,0.5,0.5);
        glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
        mat_diffuse[0] = 0.0;mat_diffuse[1] = 1.0;mat_diffuse[2] = 0.0;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_diffuse);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
        glPopMatrix();
        }
        if (num_lights > 2) {
        // Light 2
        glPushMatrix();
        glRotatef((float)i*3, 0,0,1);
        glTranslatef(0, 2.5,0);
        //glScalef(0.5,0.5,0.5);
        glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
        mat_diffuse[0] = 0.0;mat_diffuse[1] = 0.0;mat_diffuse[2] = 1.0;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_diffuse);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
        glPopMatrix();
        }
        glTranslatef(-2, 2,0);
        glVertexPointer(3, GL_FLOAT, 0, triangle);
        glEnableClientState(GL_VERTEX_ARRAY);
        glColorPointer(3, GL_FLOAT, 0, colors);
        glEnableClientState(GL_COLOR_ARRAY);
        glNormalPointer(GL_FLOAT, 0, normals);
        glEnableClientState(GL_NORMAL_ARRAY);
        //glEnable(GL_COLOR_MATERIAL);
        mat_diffuse[0] = 0.0;mat_diffuse[1] = 0.0;mat_diffuse[2] = 0.0;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_EMISSION, mat_diffuse);
        mat_diffuse[0] = 0.8;mat_diffuse[1] = 0.8;mat_diffuse[2] = 0.8;mat_diffuse[3] = 1.0;
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

        for (m = 0; m < 4; m++) {
            glPushMatrix();
            for (k = 0; k < 4; k++) {
                glPushMatrix();
                for (l = 0; l < 4; l++) {
                    glTranslatef(0, 0, -1.0*l);
                    glRotatef((float)i*speed, 0,0,1);
                    glRotatef((float)i*speed, 1,0,0);
                    glDrawArrays(GL_TRIANGLES, 0, 36);
                    glPopMatrix();
                    glPushMatrix();
                }
                glPopMatrix();
                glTranslatef(0, -1,0);
            }
            glPopMatrix();
            glTranslatef(1.0, 0, 0);
        }

        glFlush();
        wait_vsync();
		if (stop_flag ==0) {
	        i++;
			if (i >= 360) i = 0;
   	    }
     //}  // for
   } // while
}

#ifdef WITH_OPENING_SCENE

void opening_scene_white() {
    int i;
    float alpha;
    float w,h;
    int  num_fade_frames = 30;
    w = 640; h = 480;
    //backdoor_color_config(0);
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
        printf("scene %d alpha = %f\n",i,alpha);
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

//	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
//	glPopMatrix();										// Restore The Old Projection Matrix
//	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

}

#endif
