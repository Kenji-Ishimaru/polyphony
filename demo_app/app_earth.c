//=======================================================================
// Project Polyphony
//
// File:
//   app_earth.c
//
// Abstract:
//   texture demo
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
#include <string.h>
#include <math.h>
#include <GLES/gl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "hwdep.h"
#define WITH_OPENING_SCENE
//#define VERTICAL_FLIP

void planet();
void draw_earth(int j);
void draw_earth_etc(int j);

#ifdef WITH_OPENING_SCENE
void opening_scene_white();

#include "./data/boot_tex.txt"
#endif

int main(void)
{
   system_init();
   planet();

   return 0;
}

float tr_x, tr_y, tr_z;
float step, speed;
int   stop_flag, tex_kind;

#ifdef WITH_OPENING_SCENE
unsigned int tex_num[4];
#else
unsigned int tex_num[4];
#endif

// Earth data
#include "./data/planet/xfile_vtx.txt"
#include "./data/planet/xfile_nml.txt"
#include "./data/planet/xfile_tex.txt"

#include "./data/planet/earth256x256.txt"
#include "./data/planet/earth_32_f.txt"
#include "./data/planet/jupiter_32_f.txt"
#include "./data/Font.txt"

typedef struct {
    float x,y;
} coord;

coord font_coord[4];

typedef struct {
    float u,v;
} texcoord;

texcoord tex_coord[256][4];
void BuildFont()
{
    float    cx;			// Holds Our X Character Coord
    float    cy;			// Holds Our Y Character Coord
    int loop;

    font_coord[0].x = 0;
    font_coord[0].y = 0;		// Vertex Coord (Bottom Left)
    font_coord[1].x = 16;
    font_coord[1].y = 0;		// Vertex Coord (Bottom Right)
    font_coord[2].x =16;
    font_coord[2].y =16;		// Vertex Coord (Top Right)
    font_coord[3].x = 0;
    font_coord[3].y  =16;		// Vertex Coord (Top Left)
    for (loop=0; loop<256; loop++)		// Loop Through All 256 Lists
    {
        cx=(float)(loop%16)/16.0f;	// X Position Of Current Character
        cy=(float)(loop/16)/16.0f;	// Y Position Of Current Character

        tex_coord[loop][0].u = cx;
        tex_coord[loop][0].v = cy+0.0625f;        // Texture Coord (Bottom Left)
        tex_coord[loop][1].u = cx+0.0625f;
        tex_coord[loop][1].v = cy+0.0625f;	// Texture Coord (Bottom Right)
        tex_coord[loop][2].u = cx+0.0625f;
        tex_coord[loop][2].v = cy;			// Texture Coord (Top Right)
        tex_coord[loop][3].u = cx;
        tex_coord[loop][3].v = cy;					// Texture Coord (Top Left)
    }
}

void FontPrint(GLint x, GLint y, char *string, int set) {
    int size = strlen(string);
    int i,num;
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrthof(0.0, 640.0, 0.0, 480.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(x,y,0);
    glBindTexture(GL_TEXTURE_2D,tex_num[2]);
    for (i = 0; i < size; i++) {
        num = string[i];
        num -=32;
		#ifdef VERTICAL_FLIP
		glPushMatrix();
        glRotatef(-90, 0,0,1);
        #endif
        glBegin(GL_TRIANGLES);
        // tri0 0-1-2
        glTexCoord2f(tex_coord[num][0].u, tex_coord[num][0].v);	
        glVertex3f(font_coord[0].x,font_coord[0].y,-0.5);
        glTexCoord2f(tex_coord[num][1].u, tex_coord[num][1].v);	
        glVertex3f(font_coord[1].x,font_coord[1].y,-0.5);
        glTexCoord2f(tex_coord[num][2].u, tex_coord[num][2].v);	
        glVertex3f(font_coord[2].x,font_coord[2].y,-0.5);
        // tri1 2-3-0
        glTexCoord2f(tex_coord[num][2].u, tex_coord[num][2].v);	
        glVertex3f(font_coord[2].x,font_coord[2].y,-0.5);
        glTexCoord2f(tex_coord[num][3].u, tex_coord[num][3].v);	
        glVertex3f(font_coord[3].x,font_coord[3].y,-0.5);
        glTexCoord2f(tex_coord[num][0].u, tex_coord[num][0].v);	
        glVertex3f(font_coord[0].x,font_coord[0].y,-0.5);
        glEnd();
		#ifdef VERTICAL_FLIP
		glPopMatrix();
        #endif
		#ifdef VERTICAL_FLIP
		glTranslatef(0,-16,0);								// Position The Text (0,0 - Bottom Left)
        #else
		glTranslatef(16,0,0);								// Position The Text (0,0 - Bottom Left)
        #endif
    }
    glPopMatrix();		
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();			
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
}

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

void draw_earth(int j) {
    glEnable(GL_TEXTURE_2D);
    glVertexPointer(3, GL_FLOAT, 0, vtx_array);
    glNormalPointer(GL_FLOAT, 0, nml_array);
    glTexCoordPointer(2,GL_FLOAT, 0, tex_array);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    if (tex_kind == 0)
        glBindTexture(GL_TEXTURE_2D,tex_num[0]);
    else
        glBindTexture(GL_TEXTURE_2D,tex_num[3]);
   glRotatef((float)j, 0,1,0);
   glScalef(0.8, 0.8, 0.8);
   glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
}


void draw_earth_etc(int j) {
    glEnable(GL_TEXTURE_2D);
    glVertexPointer(3, GL_FLOAT, 0, vtx_array);
    glNormalPointer(GL_FLOAT, 0, nml_array);
    glTexCoordPointer(2,GL_FLOAT, 0, tex_array);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindTexture(GL_TEXTURE_2D,tex_num[1]);
    glRotatef((float)j, 0,1,0);
    glScalef(0.8, 0.8, 0.8);
    glDrawArrays(GL_TRIANGLES, 0, num_polygon*3);
}
void planet() {
    int i;
    stop_flag = 0;
    tex_kind = 0;
    gl_init();
    BuildFont();
    glClearColor(0.1, 0.1, 0.1, 1.0);
    glClearDepthf(1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Lighting configuration
    // Light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_position[] = {-1.0, 10.0, 1.0, 0.0};
    GLfloat light_ambient[]  = {0.3, 0.3, 0.3, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    // Material
    GLfloat mat_diffuse[]  = {1.0, 1.0, 1.0, 1.0};
    //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

    glEnable(GL_TEXTURE_2D);
#ifdef WITH_OPENING_SCENE
    glGenTextures(4,tex_num);
#else
    glGenTextures(4,tex_num);
#endif

    glBindTexture(GL_TEXTURE_2D,tex_num[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, earth_array);

    glBindTexture(GL_TEXTURE_2D,tex_num[1]);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 
                           0, 8192, eirth_etc_array);

	glBindTexture(GL_TEXTURE_2D,tex_num[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, font_dat);
    glBindTexture(GL_TEXTURE_2D,tex_num[3]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 
                 0, GL_RGB, GL_UNSIGNED_BYTE, jupiter_array);
				 
#ifdef WITH_OPENING_SCENE
    // texture0
    glBindTexture(GL_TEXTURE_2D,tex_num[3]);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 
                           0, 38406-6, boot_texel_array);

    opening_scene_white();
#endif
				 
    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(30.0,4.0/3.0, 1, 100);
	
    tr_x = 0;
    tr_y = 0;
    tr_z = -5;
    step = 0.05;
    i = 0;
    while(1) {
    	printf("scene = %d\n", i);
        get_input();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,0,1, 0, 0, 0, 0,1,0);
        glTranslatef(tr_x, tr_y,tr_z);
		glPushMatrix();
        glTranslatef(-1, 0,0);
#ifdef VERTICAL_FLIP
        glRotatef(-90-23, 0,0,1);
#endif
        draw_earth(i);
		glPopMatrix();
		glPushMatrix();
        glTranslatef(1, 0,0);
#ifdef VERTICAL_FLIP
        glRotatef(-90-23, 0,0,1);
#endif
        draw_earth_etc(i);
		glPopMatrix();
#ifdef VERTICAL_FLIP
		FontPrint(20,330, "Bottom:  32bit", 0); 
		FontPrint(315,330, "Top: ETC", 0); 
#else
		FontPrint(80,70, "Left:  32bit", 0); 
		FontPrint(400,70, "Right: ETC", 0); 
#endif
        glFlush();
        wait_vsync();
		if (stop_flag ==0) {
	        i++;
			if (i >= 360) i = 0;
   	    }
    }
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
    glDisable(GL_DEPTH_TEST);     // Disables Depth Testing
    glMatrixMode(GL_PROJECTION);  // Select The Projection Matrix
    glPushMatrix();		// Store The Projection Matrix
    glLoadIdentity();		// Reset The Projection Matrix
    glOrthof(0,640,0,480,-1,1);		// Set Up An Ortho Screen
    glMatrixMode(GL_MODELVIEW);	// Select The Modelview Matrix
    glPushMatrix();		// Store The Modelview Matrix
    glLoadIdentity();		// Reset The Modelview Matrix
    glBindTexture(GL_TEXTURE_2D,tex_num[3]);
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
    glPopMatrix();		// Restore The Old Projection Matrix
    glMatrixMode(GL_PROJECTION);// Select The Projection Matrix
    glPopMatrix();	// Restore The Old Projection Matrix

    glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

}

#endif
