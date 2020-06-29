//=======================================================================
// Project Polyphony
//
// File:
//   pl_vertex3.h
//
// Abstract:
//   vertex3 class header
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

#ifndef __PL_VERTEX3_H__
#define __PL_VERTEX3_H__
#include "pl_address_table.h"

typedef struct {
    float x, y, z;
} pl_vertex3;

void pl_vertex3_init(pl_vertex3 *dst);
void pl_vertex3_set0(pl_vertex3 *dst, float lx,  float ly,  float lz);
void pl_vertex3_set1(pl_vertex3 *dst, pl_vertex3 *v);

float pl_vertex3_distance(pl_vertex3 *a,  pl_vertex3 *b);
void  pl_vertex3_add0(pl_vertex3 *dst, float lx,  float ly,  float lz);
void  pl_vertex3_add1(pl_vertex3 *dst, pl_vertex3 *v);
void  pl_vertex3_show(pl_vertex3 *dst);

#endif
