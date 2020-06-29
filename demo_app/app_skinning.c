//=======================================================================
// Project Polyphony
//
// File:
//   app_skinning.c
//
// Abstract:
//   skinning demo
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
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "hwdep.h"


void skinning();
int random();

#ifdef WITH_OPENING_SCENE
void opening_scene_white();

#include "./data/boot_tex.txt"
#endif

void set_background();

#include "./data/boot_tex.txt"
unsigned int tex_num[2];


#include "./data/skin/hand_full.h"

int stop_flag;

int main(void)
{
   system_init();
   skinning();

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
    c = my_getchar();
    if (c == '9') glEnable(GL_MULTISAMPLE);
    if (c == '0') glDisable(GL_MULTISAMPLE);
    if (c == '1') {
		if (stop_flag == 0) stop_flag = 1;
		else stop_flag = 0;
	}
}

void get_input() {
    get_input_serial();
}

void skinning() {
    int i,j;
	int cframe = 1;
	int mat_index;
	float cScale = 1.0;
	stop_flag = 0;
    gl_init();

    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClearDepthf(1.0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(2, (unsigned int*)&tex_num);
    // texture0
    glBindTexture(GL_TEXTURE_2D,tex_num[0]);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 640, 480, 
                           0, 38406-6, boot_texel_array);

    glDisable(GL_TEXTURE_2D);
	
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
     // Lighting configuration
    glEnable(GL_LIGHTING);
    // Light
    glEnable(GL_LIGHT0);
    GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 0.0};
    GLfloat light0_specular[] = {0.0, 0.0, 0.0, 0.0};
    GLfloat light0_position[] = {0.0, 0.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR,  light0_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    // Material
	glShadeModel(GL_SMOOTH);

    glEnable(GL_MATRIX_PALETTE_OES);
    glMatrixMode(GL_MATRIX_PALETTE_OES);
    // Material
    GLfloat mat_ambient[]  = {0.6, 0.6, 0.6, 1.0};
    GLfloat mat_diffuse[]  = {0.0, 0.0, 1.0, 1.0};
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shininess = 50.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glViewport(0, 0, 640, 480);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(30.0,4.0/3.0, 1, 100);

    glEnable(GL_MULTISAMPLE);

    while (1) {
        glClear(GL_DEPTH_BUFFER_BIT); 
		set_background() ;
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0,0,1, 0, 0, 0, 0,1,0);
    	glTranslatef(-0.1,0.2,0);

        get_input();

        for (i=0; i<num_of_draws;i++) {
            if (num_of_weight_indexes_index[i] <=4) {
            for(j=0;j<num_of_weight_indexes_index[i];j++) {
                glCurrentPaletteMatrixOES(j);
                glLoadIdentity();
                glLoadPaletteFromModelViewMatrixOES();
                glScalef(cScale, cScale, cScale);
                mat_index = weight_matrix_reference[i][j];
                glMultMatrixf((matrix_index[mat_index])[cframe]);
            }
            // Material configuration
            int mat_num = material_no_for_each_draw[i];
            mat_diffuse[0] = materials[mat_num][0];
            mat_diffuse[1] = materials[mat_num][1];
            mat_diffuse[2] = materials[mat_num][2];
            mat_specular[0] = materials[mat_num][3];
            mat_specular[1] = materials[mat_num][4];
            mat_specular[2] = materials[mat_num][5];
            //mat_shininess = materials[mat_num][6];
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glEnableClientState(GL_MATRIX_INDEX_ARRAY_OES);
            glEnableClientState(GL_WEIGHT_ARRAY_OES);

            glVertexPointer(3, GL_FLOAT, 0, vtx_array_index[i]);
            glNormalPointer(GL_FLOAT, 0, nml_array_index[i]);
            glEnable(GL_NORMALIZE);
            glWeightPointerOES(num_of_weight_indexes_index[i], GL_FLOAT, 0,weight_array_index[i]);
            glMatrixIndexPointerOES(num_of_weight_indexes_index[i], GL_UNSIGNED_BYTE, 0, weight_index_array_index[i]);
            glDrawArrays(GL_TRIANGLES, 0, num_of_tri_array_index[i]*3);
            }
        }
        glFlush();
        wait_vsync();
		if (stop_flag ==0) {
	        cframe++;
	        if (cframe >= num_of_frames) cframe = 1;
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

//	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
//	glPopMatrix();										// Restore The Old Projection Matrix
//	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
//	glPopMatrix();										// Restore The Old Projection Matrix
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);

}

#endif


void set_background() {
    float alpha = 1.0;

    float w,h;
    w = 640; h = 480;
    //backdoor_color_config(0);
    glShadeModel(GL_SMOOTH);

    glEnable(GL_TEXTURE_2D);
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
    glDisable(GL_TEXTURE_2D);

}
