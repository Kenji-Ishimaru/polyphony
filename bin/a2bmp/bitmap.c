//=======================================================================
// Project Polyphony
//
// File:
//   a2bmp.c
//
// Abstract:
//   ascii to bitmap converter
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
// Revision History

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include"bitmap.h"


Image *read_txt(char *filename, int w, int h)
{
  int i, j;
  unsigned int color;
  FILE *fp;
  Image *img;

  if((fp = fopen(filename, "rb")) == NULL){
    fprintf(stderr, "Error: %s not found.", filename);
    return NULL;
  }
  // File format ABGR
  printf("w=%d, h=%d\n",w,h);

  if((img = create_image(w, h)) == NULL) return NULL;
  for (i=0;i<h;i++) {
    for (j=0;j<w;j++) {
      fscanf(fp, "%08x", &color);
      img->dat[w*i+j].r = color & 0xff;
      img->dat[w*i+j].g = (color >> 8) & 0xff;
      img->dat[w*i+j].b = (color >> 16) & 0xff;
    }
  }

  fclose(fp);

  return img;
}

int write_bmp(char *filename, Image *img)
{
  int i, j;
  FILE *fp;
  int real_width;
  unsigned char *bmp_line_data;
  unsigned char header_buf[HEADERSIZE];
  unsigned int file_size;
  unsigned int offset_to_data;
  unsigned long info_header_size;
  unsigned int planes;
  unsigned int color;
  unsigned long compress;
  unsigned long data_size;
  long xppm;
  long yppm;

  if((fp = fopen(filename, "wb")) == NULL){
    fprintf(stderr, "Error: %s could not open.", filename);
    return 1;
  }

  real_width = img->width*3 + img->width%4;

  // create header
  file_size = img->height * real_width + HEADERSIZE;
  offset_to_data = HEADERSIZE;
  info_header_size = INFOHEADERSIZE;
  planes = 1;
  color = 24;
  compress = 0;
  data_size = img->height * real_width;
  xppm = 1;
  yppm = 1;
	
  header_buf[0] = 'B';
  header_buf[1] = 'M';
  memcpy(header_buf + 2, &file_size, sizeof(file_size));
  header_buf[6] = 0;
  header_buf[7] = 0;
  header_buf[8] = 0;
  header_buf[9] = 0;
  memcpy(header_buf + 10, &offset_to_data, sizeof(file_size));
  header_buf[11] = 0;
  header_buf[12] = 0;
  header_buf[13] = 0;

  memcpy(header_buf + 14, &info_header_size, sizeof(info_header_size));
  header_buf[15] = 0;
  header_buf[16] = 0;
  header_buf[17] = 0;
  memcpy(header_buf + 18, &img->width, sizeof(img->width));
  memcpy(header_buf + 22, &img->height, sizeof(img->height));
  memcpy(header_buf + 26, &planes, sizeof(planes));
  memcpy(header_buf + 28, &color, sizeof(color));
  memcpy(header_buf + 30, &compress, sizeof(compress));
  memcpy(header_buf + 34, &data_size, sizeof(data_size));
  memcpy(header_buf + 38, &xppm, sizeof(xppm));
  memcpy(header_buf + 42, &yppm, sizeof(yppm));
  header_buf[46] = 0;
  header_buf[47] = 0;
  header_buf[48] = 0;
  header_buf[49] = 0;
  header_buf[50] = 0;
  header_buf[51] = 0;
  header_buf[52] = 0;
  header_buf[53] = 0;

  // write header
  fwrite(header_buf, sizeof(unsigned char), HEADERSIZE, fp);
	
  if((bmp_line_data = (unsigned char *)malloc(sizeof(unsigned char)*real_width)) == NULL){
    fprintf(stderr, "Error: Allocation error.\n");
    fclose(fp);
    return 1;
  }

  // write RGB
  for(i=0; i<img->height; i++){
    for(j=0; j<img->width; j++){
      bmp_line_data[j*3] = img->dat[(img->height - i - 1)*img->width + j].b;
      bmp_line_data[j*3 + 1] = img->dat[(img->height - i - 1)*img->width + j].g;
      bmp_line_data[j*3 + 2] = img->dat[(img->height - i - 1)*img->width + j].r;
    }
    // 4-bite align
    for(j=img->width*3; j<real_width; j++){
      bmp_line_data[j] = 0;
    }
    fwrite(bmp_line_data, sizeof(unsigned char), real_width, fp);
  }
  free(bmp_line_data);

  fclose(fp);
  return 0;
}

Image *create_image(int width, int height)
{
  Image *img;

  if((img = (Image *)malloc(sizeof(Image))) == NULL){
    fprintf(stderr, "Allocation error\n");
    return NULL;
  }

  if((img->dat = (rgb*)malloc(sizeof(rgb)*width*height)) == NULL){
    fprintf(stderr, "Allocation error\n");
      free(img);
      return NULL;
  }

  img->width = width;
  img->height = height;

  return img;
}

void delete_image(Image *img)
{
  free(img->dat);
  free(img);
}

