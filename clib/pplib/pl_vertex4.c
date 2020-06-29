//=======================================================================
// Project Polyphony
//
// File:
//   pl_pl_vertex4.c
//
// Abstract:
//   pl_vertex4 class header
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

#include "pl_vertex4.h"
#include "pplib.h"

extern t_fui u_fui;

void pl_vertex4_init(pl_vertex4 *dst) {
    dst->x = 0.0;
    dst->y = 0.0;
    dst->z = 0.0;
    dst->w = 0.0;
}

void pl_vertex4_set0(pl_vertex4 *dst, float lx,  float ly,  float lz) {
    dst->x = lx;
    dst->y = ly;
    dst->z = lz;
    dst->w = 1.0;
}

void pl_vertex4_set1(pl_vertex4 *dst, float lx,  float ly,
                      float lz,  float lw) {
    dst->x = lx;
    dst->y = ly;
    dst->z = lz;
    dst->w = lw;
}

void pl_vertex4_set2(pl_vertex4 *dst, pl_vertex4 *v) {
    dst->x = v->x;
    dst->y = v->y;
    dst->z = v->z;
    dst->w = v->w;
}

void pl_vertex4_set3(pl_vertex4 *dst, unsigned int b[]) {
    u_fui.ui= b[0];
    dst->x = u_fui.f;
    u_fui.ui= b[1];
    dst->y = u_fui.f;
    u_fui.ui= b[2];
    dst->z = u_fui.f;
    u_fui.ui= b[3];
    dst->w = u_fui.f;
}



