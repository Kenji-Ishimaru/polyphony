//=======================================================================
// Project Polyphony
//
// File:
//   pl_vector3.c
//
// Abstract:
//   vector3 class implementation
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

#include <math.h>
#include "pl_address_table.h"
#include "pl_vector3.h"

void pl_vector3_pl_vector3(pl_vector3 *dst) {
    dst->x = 0.0;
    dst->y = 0.0;
    dst->z = 0.0;
}

void pl_vector3_set0(pl_vector3 *dst, float lx,  float ly,  float lz) {
    dst->x = lx;
    dst->y = ly;
    dst->z = lz;
}

void pl_vector3_set1(pl_vector3 *dst, pl_vector3 *v) {
    dst->x = v->x; 
    dst->y = v->y; 
    dst->z = v->z; 
}

void pl_vector3_set2(pl_vector3 *dst, pl_vertex3 *v) {
    dst->x = v->x; 
    dst->y = v->y; 
    dst->z = v->z; 
}


int pl_vector3_is_zero_vector(pl_vector3 *dst) {
    if (dst->x < 0 &&
        dst->y < 0 &&
        dst->z < 0) return 1;  // true

    return 0;  // false
}

float pl_vector3_size(pl_vector3 *dst) {
    float t = dst->x*dst->x + dst->y*dst->y + dst->z*dst->z;
    t = sqrt(t);
    return t;
}

float pl_vector3_size2(pl_vector3 *dst) {
    float t = dst->x*dst->x + dst->y*dst->y + dst->z*dst->z;
    return t;
}

void pl_vector3_normalize(pl_vector3 *dst) {
    float t = pl_vector3_size(dst);
    if (t <= 0) {
        printf("pl_vector3 Divide by Zero");
        printf("xyz = %f %f %f\n",dst->x,dst->y,dst->z);
        dst->x = 0;
        dst->y = 0;
        dst->z = 0;
    } else {
      t = 1.0F/t;
      dst->x *= t;
      dst->y *= t;
      dst->z *= t;
    }
}

void pl_vector3_cross_product0(pl_vector3 *dst, pl_vector3 *a,  pl_vector3 *b){
    dst->x = a->y*b->z - b->y*a->z;
    dst->y = a->z*b->x - b->z*a->x;
    dst->z = a->x*b->y - b->x*a->y;
}

void pl_vector3_cross_product1(pl_vector3 *dst, pl_vector3 *b) {
    dst->x = dst->y*b->z - b->y*dst->z;
    dst->y = dst->z*b->x - b->z*dst->x;
    dst->z = dst->x*b->y - b->x*dst->y;
}

float pl_vector3_inner_product0(pl_vector3 *dst, pl_vector3 *a) {
    float t = dst->x*a->x + dst->y*a->y + dst->z*a->z;
    return t;
}

float pl_vector3_inner_product1(float x,  float y,  float z){
    float t = x*x + y*y + z*z;
    return t;
}

float pl_vector3_inner_product2(pl_vector3 *dst, pl_vertex3 *a) {
    float t = dst->x*a->x + dst->y*a->y + dst->z*a->z;
    return t;
}

float pl_vector3_inner_product3( pl_vector3 *a,  pl_vector3 *b) {
    float t = a->x*b->x+a->y*b->y+a->z*b->z;
    return t;
}

void pl_vector3_scale(pl_vector3 *dst, float t) {
    dst->x *= t;
    dst->y *= t;
    dst->z *= t;
}

void pl_vector3_add0(pl_vector3 *dst, pl_vector3 *v) {
    dst->x += v->x;
    dst->y += v->y;
    dst->z += v->z;
}

void pl_vector3_add1(pl_vector3 *dst, pl_vertex3 *v) {
    dst->x += v->x;
    dst->y += v->y;
    dst->z += v->z;
}

void pl_vector3_subtract0(pl_vector3 *dst, pl_vector3 *v) {
    dst->x -= v->x;
    dst->y -= v->y;
    dst->z -= v->z;
}

void pl_vector3_subtract1(pl_vector3 *dst, pl_vertex3 *v) {
    dst->x -= v->x;
    dst->y -= v->y;
    dst->z -= v->z;
}

void pl_vector3_assign0(pl_vector3 *dst, pl_vector3 *v) {
    dst->x = v->x;
    dst->y = v->y;
    dst->z = v->z;
}

void pl_vector3_assign1(pl_vector3 *dst, pl_vertex3 *v) {
    dst->x = v->x;
    dst->y = v->y;
    dst->z = v->z;
}
