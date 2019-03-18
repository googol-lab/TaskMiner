#include <omp.h>
#ifndef taskminerutils
#define taskminerutils
static int taskminer_depth_cutoff = 0;
#define DEPTH_CUTOFF omp_get_num_threads()
extern char cutoff_test;
#endif
/*
 *  An implementation of the ARC4 algorithm
 *
 *  Copyright (C) 2001-2003  Christophe Devine
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdlib.h>
#include "rc4.h"

void rc4_setup(struct rc4_state *s, unsigned char *key, int length) {
  int i, j, k, *m, a;

  s->x = 0;
  s->y = 0;
  m = s->m;

  for (i = 0; i < 256; i++) {
    m[i] = i;
  }

  j = k = 0;

  for (i = 0; i < 256; i++) {
    a = m[i];
    j = (unsigned char)(j + a + key[k]);
    m[i] = m[j];
    m[j] = a;
    if (++k >= length)
      k = 0;
  }
}

void rc4_crypt(struct rc4_state *s, unsigned char *data, int length) {
  int i, x, y, *m, a, b;

  x = s->x;
  y = s->y;
  m = s->m;

  for (i = 0; i < length; i++) {
    x = (unsigned char)(x + 1);
    a = m[x];
    y = (unsigned char)(y + a);
    m[x] = b = m[y];
    m[y] = a;
    data[i] ^= m[(unsigned char)(a + b)];
  }

  s->x = x;
  s->y = y;
}

#include <string.h>
#include <stdio.h>

/*
 * ARC4 tests vectors from OpenSSL (crypto/rc4/rc4test.c)
 */

static unsigned char keys[7][30] = {
    {8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef},
    {8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef},
    {8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {4, 0xef, 0x01, 0x23, 0x45},
    {8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef},
    {4, 0xef, 0x01, 0x23, 0x45},
};

static unsigned char data_len[7] = {8, 8, 8, 20, 28, 10};
static unsigned char data[7][30] = {
    {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xff},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
     0x00, 0x00, 0x00, 0x00, 0xff},
    {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
     0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
     0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0,
     0x12, 0x34, 0x56, 0x78, 0xff},
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff},
    {0},
};

static unsigned char output[7][30] = {
    {0x75, 0xb7, 0x87, 0x80, 0x99, 0xe0, 0xc5, 0x96, 0x00},
    {0x74, 0x94, 0xc2, 0xe7, 0x10, 0x4b, 0x08, 0x79, 0x00},
    {0xde, 0x18, 0x89, 0x41, 0xa3, 0x37, 0x5d, 0x3a, 0x00},
    {0xd6, 0xa1, 0x41, 0xa7, 0xec, 0x3c, 0x38, 0xdf,
     0xbd, 0x61, 0x5a, 0x11, 0x62, 0xe1, 0xc7, 0xba,
     0x36, 0xb6, 0x78, 0x58, 0x00},
    {0x66, 0xa0, 0x94, 0x9f, 0x8a, 0xf7, 0xd6, 0x89,
     0x1f, 0x7f, 0x83, 0x2b, 0xa8, 0x33, 0xc0, 0x0c,
     0x89, 0x2e, 0xbe, 0x30, 0x14, 0x3c, 0xe2, 0x87,
     0x40, 0x01, 0x1e, 0xcf, 0x00},
    {0xd6, 0xa1, 0x41, 0xa7, 0xec, 0x3c, 0x38, 0xdf, 0xbd, 0x61, 0x00},
    {0},
};

int main(int argc, const char **argv) {
  int i, j;
  struct rc4_state s;
  unsigned char buffer[30];

  int count = 200000;
  if (argc == 2)
    count = atoi(argv[1]);

  printf("\n RC4 Validation Tests:\n\n");

  #pragma omp parallel
  #pragma omp single
  for (i = 0; i < 6; i++) {
    printf(" Test %d ", i + 1);

    #pragma omp parallel
    #pragma omp single
    for (j = 0; j < count; j++) {

      memcpy(buffer, data[i], data_len[i]);

      rc4_setup(&s, &keys[i][1], keys[i][0]);
      {
      cutoff_test = (taskminer_depth_cutoff < DEPTH_CUTOFF);
      #pragma omp task untied default(shared) shared(
      {
      rc4_crypt(&s, buffer, data_len[i]);
     }
    }
#pragma omp taskwait

    if (memcmp(buffer, output[i], data_len[i])) {
      printf("failed!\n");
      return (1);
    }

    printf("passed.\n");
  }

  printf("\n");

  return (0);
}

