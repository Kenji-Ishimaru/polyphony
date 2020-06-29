//=======================================================================
// Project Polyphony
//
// File:
//   pl_matrix3.h
//
// Abstract:
//   matrix3 class header
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

#ifndef __PL_MATRIX3_H__
#define __PL_MATRIX3_H__


#include "pl_address_table.h"
#include "pl_vector3.h"
#include "pl_vector4.h"


typedef struct {
    float a[9];
} pl_matrix3;

// functions
void pl_matrix3_init(pl_matrix3 *dst);
void pl_matrix3_set0(pl_matrix3 *dst, float b[]);
void pl_matrix3_set1(pl_matrix3 *dst, float a0,  float a1,  float a2,
                                      float a3,  float a4,  float a5,
                                      float a6,  float a7,  float a8 );
void pl_matrix3_set2(pl_matrix3 *dst, pl_matrix3 *t);

float pl_matrix3_determinant0(pl_matrix3 *dst);
float pl_matrix3_determinant1(
         float a0,  float a1,  float a2,
         float a3,  float a4,  float a5,
         float a6,  float a7,  float a8);
void pl_matrix3_multiply_vector30(pl_vector3 *dst,pl_matrix3 *m,  pl_vector3 *v);
void pl_matrix3_multiply_vector31(pl_vertex3 *dst,pl_matrix3 *m,  pl_vertex3 *v);
void pl_matrix3_multiply(pl_matrix3 *dst, pl_matrix3 *b);
void pl_matrix3_inverse(pl_matrix3 *dst);
void pl_matrix3_rotate0(pl_matrix3 *dst, pl_vector3 *v,  float theta);
void pl_matrix3_rotate1(pl_matrix3 *dst, float dx,  float dy,
                                         float dz,  float theta);
void pl_matrix3_rotate2(pl_matrix3 *dst, pl_vector4 *v);
void show(pl_matrix3 *dst);
    // | a[0] a[1] a[2] |
    // | a[3] a[4] a[5] |
    // | a[6] a[7] a[8] |

#endif
