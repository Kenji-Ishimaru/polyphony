//=======================================================================
// Project Polyphony
//
// File:
//   pli_src.c
//
// Abstract:
//   verilog PLI 
//
//  Created:
//    6 October 2008
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

#include "acc_user.h"
#include "veriuser.h"

// float32 to float 24
//   float32 : s = 1bit, e = 8bit, m = 23bit (bias:127)
//   float24 : s = 1bit, e = 7bit, m = 16bit (bias:63)
unsigned int cnv_f32_to_f24(unsigned int a) {
  // extract sign
  unsigned int tmp_s = (a >> 31) & 1;
  // extract exp
  unsigned int tmp_e = (a >> 23) & 0xff;
  if (tmp_e > 63) {
    tmp_e -= 127;
    tmp_e += 63;
  } else {
    tmp_e = 0;
  }
  // extract fraction
  unsigned int tmp_m = (a >> 7)& 0xffff;
  return  (tmp_s << 23) | (tmp_e << 16) | tmp_m;
}

// float24 to float 32
//   float24 : s = 1bit, e = 7bit, m = 16bit (bias:63)
//   float32 : s = 1bit, e = 8bit, m = 23bit (bias:127)
unsigned int cnv_f24_to_f32(unsigned int a) {
  // extract sign
  unsigned int tmp_s = (a >> 23) & 1;
  // extract exp
  unsigned int tmp_e = (a >> 16) & 0x7f;
  if (tmp_e != 0) {
    tmp_e -= 63;
    tmp_e += 127;
  }
  // extract fraction
  unsigned int tmp_m = a & 0xffff;
  return  (tmp_s << 31) | (tmp_e << 23) | (tmp_m << 7);
}

int to_float32() {
  union {
    float f;
    unsigned int u;
  } _uf;

  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  static s_setval_value value_s = {accIntVal};
  delay_s.time.real = 0;

  handle reg = acc_handle_tfarg(1);  // get register handle
  float val = acc_fetch_tfarg(2);    // get floating number
  // call convert function
  _uf.f = val;
  value_s.value.integer = _uf.u;

  acc_set_value(reg,&value_s,&delay_s);
}

int disp_float32() {
  union {
    float f;
    unsigned int u;
  } _uf;
  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  static s_setval_value value_s = {accIntVal};
  delay_s.time.real = 0;

  handle obj = acc_handle_tfarg(1);     // get register handle
  acc_fetch_value(obj, "%%",&value_s);
  _uf.u = value_s.value.integer;
  char *str_disp = acc_fetch_tfarg_str(2); // get string
  io_printf("%s %06x[%f]\n",str_disp,value_s.value.integer, _uf.f);
}

int to_real32() {
  union {
    float f;
    unsigned int u;
  } _uf;

  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  delay_s.time.real = 0;
  static s_setval_value value_s = {accIntVal};

  static s_setval_delay delay_ss = {accRealTime, accNoDelay};
  delay_ss.time.real = 0;
  static s_setval_value value_ss = {accRealVal};
 
  handle reg = acc_handle_tfarg(2);  // get real handle
  acc_fetch_value(reg, "%%",&value_s);
  // call convert function
  _uf.u = value_s.value.integer;
  //  io_printf("%06x[%f]\n",value_s.value.integer, _uf.f);
  handle rrl = acc_handle_tfarg(1);  // get real handle

  value_ss.value.real = (double)_uf.f;

  acc_set_value(rrl,&value_ss,&delay_s);
}

int to_float24() {
  union {
    float f;
    unsigned int u;
  } _uf;

  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  static s_setval_value value_s = {accIntVal};
  delay_s.time.real = 0;

  handle reg = acc_handle_tfarg(1);  // get register handle
  float val = acc_fetch_tfarg(2);    // get floating number
  // call convert function
  _uf.f = val;
  value_s.value.integer = cnv_f32_to_f24(_uf.u);

  acc_set_value(reg,&value_s,&delay_s);
}

int disp_float24() {
  union {
    float f;
    unsigned int u;
  } _uf;
  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  static s_setval_value value_s = {accIntVal};
  delay_s.time.real = 0;

  handle obj = acc_handle_tfarg(1);     // get register handle
  acc_fetch_value(obj, "%%",&value_s);
  _uf.u = cnv_f24_to_f32(value_s.value.integer);
  char *str_disp = acc_fetch_tfarg_str(2); // get string
  io_printf("%s %06x[%f]\n",str_disp,value_s.value.integer, _uf.f);
}

int to_real24() {
  union {
    float f;
    unsigned int u;
  } _uf;

  // initialize tasks
  acc_initialize();
  static s_setval_delay delay_s = {accRealTime, accNoDelay};
  delay_s.time.real = 0;
  static s_setval_value value_s = {accIntVal};

  static s_setval_delay delay_ss = {accRealTime, accNoDelay};
  delay_ss.time.real = 0;
  static s_setval_value value_ss = {accRealVal};
 
  handle reg = acc_handle_tfarg(2);  // get real handle
  acc_fetch_value(reg, "%%",&value_s);
  // call convert function
  _uf.u = cnv_f24_to_f32(value_s.value.integer);
  //  io_printf("%06x[%f]\n",value_s.value.integer, _uf.f);
  handle rrl = acc_handle_tfarg(1);  // get real handle

  value_ss.value.real = (double)_uf.f;

  acc_set_value(rrl,&value_ss,&delay_s);
}

p_tfcell my_bootstrap () {
  static s_tfcell my_tfs[17] = {
    { usertask, 0, 0, 0, to_float32, 0, "$to_float32",1 },
    { usertask, 0, 0, 0, to_real32, 0, "$to_real32",1 },
    { usertask, 0, 0, 0, disp_float32, 0, "$disp_float32",1 },
    { usertask, 0, 0, 0, to_float24, 0, "$to_float24",1 },
    { usertask, 0, 0, 0, to_real24, 0, "$to_real24",1 },
    { usertask, 0, 0, 0, disp_float24, 0, "$disp_float24",1 },
    {0}
  };
  io_printf("*** Registering user PLI 1.0 task\n");
  return(my_tfs);
}
