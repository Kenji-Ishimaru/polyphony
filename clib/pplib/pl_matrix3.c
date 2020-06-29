//=======================================================================
// Project Polyphony
//
// File:
//   pl_matrix3.c
//
// Abstract:
//   matrix3 class implementation
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

#include <stdio.h>
#include <math.h>
#include "pl_vertex3.h"
#include "pl_matrix3.h"
#include "pl_address_table.h"

void pl_matrix3_init(pl_matrix3 *dst) {
    int i;
    for (i=0;i<9;i++) dst->a[i] = 0.0;
    dst->a[0] = dst->a[4] = dst->a[8] = 1.0;
}

void pl_matrix3_set0(pl_matrix3 *dst, float b[]) {
    int i;
    for (i=0;i<9;i++) dst->a[i] = b[i];
}

void pl_matrix3_set1(pl_matrix3 *dst,
                               float a0,  float a1,  float a2,
                               float a3,  float a4,  float a5,
                               float a6,  float a7,  float a8 ) {
		dst->a[0] = a0; dst->a[1] = a1; dst->a[2] = a2;
		dst->a[3] = a3; dst->a[4] = a4; dst->a[5] = a5;
		dst->a[6] = a6; dst->a[7] = a7; dst->a[8] = a8;

}

void pl_matrix3_set2(pl_matrix3 *dst, pl_matrix3 *t) {
    int i;
    for (i=0;i<9;i++) dst->a[i] = t->a[i];
}

float pl_matrix3_determinant0(pl_matrix3 *dst) {
    float t =
        dst->a[0]*dst->a[4]*dst->a[8] +
        dst->a[1]*dst->a[5]*dst->a[6] +
        dst->a[2]*dst->a[7]*dst->a[3] -
        dst->a[6]*dst->a[4]*dst->a[2] -
        dst->a[5]*dst->a[7]*dst->a[0] -
        dst->a[8]*dst->a[3]*dst->a[1];
    return t;
}

float pl_matrix3_determinant1(
         float a0,  float a1,  float a2,
         float a3,  float a4,  float a5,
         float a6,  float a7,  float a8) {
    float t =
        a0*a4*a8 +
        a1*a5*a6 +
        a2*a7*a3 -
        a6*a4*a2 -
        a5*a7*a0 -
        a8*a3*a1;
    return t;
}

void pl_matrix3_multiply_vector30(pl_vector3 *dst, pl_matrix3 *m,  pl_vector3 *v) {
    dst->x = m->a[0]*v->x + m->a[1]*v->y + m->a[2]*v->z;
    dst->y = m->a[3]*v->x + m->a[4]*v->y + m->a[5]*v->z;
    dst->z = m->a[6]*v->x + m->a[7]*v->y + m->a[8]*v->z;
}

void pl_matrix3_multiply_vector31(pl_vertex3 *dst, pl_matrix3 *m,  pl_vertex3 *v) {
    dst->x = m->a[0]*v->x + m->a[1]*v->y + m->a[2]*v->z;
    dst->y = m->a[3]*v->x + m->a[4]*v->y + m->a[5]*v->z;
    dst->z = m->a[6]*v->x + m->a[7]*v->y + m->a[8]*v->z;
}

void pl_matrix3_multiply(pl_matrix3 *dst, pl_matrix3 *b) {
    pl_matrix3 c;
    int i,j,k,p,q,r,s;
    for ( k=0 , p=0, q=0 ; k < 3 ; k++, q += 3 ){
        for ( j=0 ; j < 3 ; j++ ){
            r = q;
            s = j;
            c.a[p] = 0;
            for ( i=0 ; i < 3 ; i++, r++, s += 3 ){
                c.a[p] += b->a[r] * dst->a[s];
            }
            p++;
        }
    }
    for (k = 0; k < 9 ; k++) dst->a[k] = c.a[k];
}

void pl_matrix3_inverse(pl_matrix3 *dst) {
    float t = pl_matrix3_determinant0(dst);
    if (fabs(t) <= 0) printf("pl_matrix3 Divide by Zero\n");
    t = 1.0F/t;
    float b[9];
    /*
    |  |a[4] a[5]|   |a[3] a[5]|   |a[3] a[4]| |
    |  |a[7] a[8]|  -|a[6] a[8]|   |a[6] a[7]| |
    |                                          |
    |  |a[1] a[2]|   |a[0] a[2]|   |a[0] a[1]| |
    | -|a[7] a[8]|   |a[6] a[8]|  -|a[6] a[7]| |
    |                                          |
    |  |a[1] a[2]|   |a[0] a[2]|   |a[0] a[1]| |
    |  |a[4] a[5]|  -|a[3] a[5]|   |a[3] a[4]| |
    */
    b[0] =  (dst->a[4]*dst->a[8]-dst->a[5]*dst->a[7])*t;
    b[1] = -(dst->a[1]*dst->a[8]-dst->a[7]*dst->a[2])*t;
    b[2] =  (dst->a[1]*dst->a[5]-dst->a[2]*dst->a[4])*t;
    b[3] = -(dst->a[3]*dst->a[8]-dst->a[5]*dst->a[6])*t;
    b[4] =  (dst->a[0]*dst->a[8]-dst->a[2]*dst->a[6])*t;
    b[5] = -(dst->a[0]*dst->a[5]-dst->a[2]*dst->a[3])*t;
    b[6] =  (dst->a[3]*dst->a[7]-dst->a[4]*dst->a[6])*t;
    b[7] = -(dst->a[0]*dst->a[7]-dst->a[1]*dst->a[6])*t;
    b[8] =  (dst->a[0]*dst->a[4]-dst->a[1]*dst->a[3])*t;

    pl_matrix3_set0(dst,b);
}

void pl_matrix3_rotate0(pl_matrix3 *dst, pl_vector3 *v,  float theta) {
    pl_vector3 w;
    pl_vector3_assign0(&w,v);
    pl_vector3_normalize(&w);
    float s = sin(theta);
    float c = cos(theta);
    float t1 = 1.0;
    float t = t1-c;

    dst->a[0] = t*w.x*w.x + c;
    dst->a[1] = t*w.x*w.y - w.z*s;
    dst->a[2] = t*w.z*w.x + w.y*s;
    dst->a[3] = t*w.x*w.y + w.z*s;
    dst->a[4] = t*w.y*w.y + c;
    dst->a[5] = t*w.y*w.z - w.x*s;
    dst->a[6] = t*w.z*w.x - w.y*s;
    dst->a[7] = t*w.y*w.z + w.x*s;
    dst->a[8] = t*w.z*w.z + c;

    /*
    a[0] = t*w.x*w.x + c;
    a[1] = t*w.x*w.y + w.z*s;
    a[2] = t*w.z*w.x - w.y*s;
    a[3] = t*w.x*w.y - w.z*s;
    a[4] = t*w.y*w.y + c;
    a[5] = t*w.y*w.z + w.x*s;
    a[6] = t*w.z*w.x + w.y*s;
    a[7] = t*w.y*w.z - w.x*s;
    a[8] = t*w.z*w.z + c;
    */
    /*
    a[0] = c;
    a[1] = -s;
    a[2] = 0;
    a[3] = s;
    a[4] = c;
    a[5] = 0;
    a[6] = 0;
    a[7] = 0;
    a[8] = 1;
    */
}

void pl_matrix3_rotate1(pl_matrix3 *dst, float dx,  float dy,
                                         float dz,  float theta) {
    pl_vector3 v;
    pl_vector3_set0(&v, dx,dy,dz);
    pl_matrix3_rotate0(dst, &v,theta);
}

void pl_matrix3_rotate2(pl_matrix3 *dst, pl_vector4 *v) {
    pl_matrix3_rotate1(dst, v->x,v->y,v->z,v->w);
}

void pl_matrix3_show(pl_matrix3 *dst) {
  printf("pl_matrix3 ");
  printf(" %f %f %f\n",dst->a[0],dst->a[1],dst->a[2]);
  printf(" %f %f %f\n",dst->a[3],dst->a[4],dst->a[5]);
  printf(" %f %f %f\n",dst->a[6],dst->a[7],dst->a[8]);
}
