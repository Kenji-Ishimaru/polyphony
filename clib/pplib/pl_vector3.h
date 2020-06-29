//=======================================================================
// Project Polyphony
//
// File:
//   pl_vector3.h
//
// Abstract:
//   vector3 class header
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

#ifndef __PL_VECTOR3_H__
#define __PL_VECTOR3_H__

#include "pl_address_table.h"
#include "pl_vertex3.h"

typedef struct {
    float x, y, z;
}  pl_vector3;


void pl_vector3_init(pl_vector3 *dst);
void pl_vector3_set0(pl_vector3 *dst,  float lx,  float ly,  float lz);
void pl_vector3_set1(pl_vector3 *dst,  pl_vector3 *v);
void pl_vector3_set2(pl_vector3 *dst,  pl_vertex3 *v);

int pl_vector3_is_zero_vector(pl_vector3 *dst);
float pl_vector3_size(pl_vector3 *dst);
float pl_vector3_size2(pl_vector3 *dst);
void pl_vector3_normalize(pl_vector3 *dst);
void pl_vector3_cross_product0(pl_vector3 *dst, pl_vector3 *a,  pl_vector3 *b);
void pl_vector3_cross_product1(pl_vector3 *dst, pl_vector3 *b);
float pl_vector3_inner_product0(pl_vector3 *dst, pl_vector3 *a);
float pl_vector3_inner_product1(float x,  float y,  float z);
float pl_vector3_inner_product2(pl_vector3 *dst, pl_vertex3 *a);
float pl_vector3_inner_product3( pl_vector3 *a,  pl_vector3 *b);
void pl_vector3_scale(pl_vector3 *dst, float t);
void pl_vector3_add0(pl_vector3 *dst, pl_vector3 *v);
void pl_vector3_add1(pl_vector3 *dst, pl_vertex3 *v);
void pl_vector3_subtract0(pl_vector3 *dst, pl_vector3 *v);
void pl_vector3_subtract1(pl_vector3 *dst, pl_vertex3 *v);
void pl_vector3_assign0(pl_vector3 *dst, pl_vector3 *v);
void pl_vector3_assign1(pl_vector3 *dst, pl_vertex3 *v);

#endif
