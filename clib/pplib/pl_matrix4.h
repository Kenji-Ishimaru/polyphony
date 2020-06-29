//=======================================================================
// Project Polyphony
//
// File:
//   pl_matrix4.h
//
// Abstract:
//   4x4 matrix class header
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

#ifndef __PL_MATRIX4_H__
#define __PL_MATRIX4_H__

#include "pl_vector3.h"
#include "pl_vector4.h"
#include "pl_matrix3.h"
#include "pl_address_table.h"

typedef struct {
    float a[16];
    float at[4][4];  // transpozed a
} pl_matrix4;

// functions
void pl_matrix4_init(pl_matrix4 *dst);
void pl_matrix4_set0(pl_matrix4 *dst, float b[]);
void pl_matrix4_set1(pl_matrix4 *dst, unsigned int b[]);
void pl_matrix4_set2(pl_matrix4 *dst, pl_matrix4 *t);
void pl_matrix4_set3(pl_matrix4 *dst, const float *b);

void pl_matrix4_identity(pl_matrix4 *dst);
void pl_matrix4_assign(pl_matrix4 *dst, pl_matrix4 *v);
void pl_matrix4_multiply_vector4(pl_vector4 *dst, pl_matrix4 *m, pl_vector4 *v);
void pl_matrix4_multiply_vertex4(pl_vertex4 *dst, pl_matrix4 *m, pl_vertex4 *v);
void pl_matrix4_multiply_f(pl_matrix4 *m, float *v);
void pl_matrix4_multiply_f2(pl_matrix4 *m, float *dst ,float *src);
void pl_matrix4_multiply_f2_n(pl_matrix4 *m, float *dst ,float *src);

void pl_matrix4_multiply_matrix4(pl_matrix4 *dst, pl_matrix4 *mm, pl_matrix4 *b);
void pl_matrix4_multiply_matrix4i(pl_matrix4 *dst, pl_matrix4 *mm, pl_matrix4 *b);
void pl_matrix4_transpose(pl_matrix4 *dst,pl_matrix4 *src);
void pl_matrix4_transpose_element(pl_matrix4 *src);
void pl_matrix4_inverse(pl_matrix4 *dst,pl_matrix4 *src);
void pl_matrix4_rotate(pl_matrix4 *dst, pl_vector3 *v,  float theta);
void pl_matrix4_get_elements(pl_matrix4 *dst, unsigned int *b);
void pl_matrix4_show(pl_matrix4 *dst);

    // | a[0]  a[1]  a[2]  a[3] |
    // | a[4]  a[5]  a[6]  a[7] |
    // | a[8]  a[9]  a[10] a[11]|
    // | a[12] a[13] a[14] a[15]|


#endif
