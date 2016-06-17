#include <stdio.h>
#include <stdlib.h>

#include <vector>

#include "dlug.h"

//                       0 1 2 3 4 5 6 7 8
int dlug_bytelen[]    = {1,2,3,4,5,6,8,9,17};
int dlug_pfxbitlen[]  = {1,2,3,5,5,5,8,8,8};

//                           0   1    2    3    4    5    6    7   8
int dlug_bitlen[]         = {7,  14,  21,  27,  35,  43,  56,  64, 128};
unsigned char dlug_pfx[]  = {0,0x80,0xc0,0xe0,0xe8,0xf0,0xf8,0xf9,0xfa,0xff};



/*
int dlug_index(unsigned char *d) {
  int i;
  for (i=0; i<9; i++) {
    if ((d[0] & (0xff << (8-dlug_pfxbitlen[i]))) == dlug_pfx[i]) {
      return i;
    }
  }
  return -2;
}
*/

inline int dlug_index(unsigned char *d) {
  int i;
  for (i=0; i<9; i++) {
    if ((d[0] & (0xff << (8-dlug_pfxbitlen[i]))) == dlug_pfx[i]) {
      return i;
    }
  }
  return -2;
}

int dlug_len(unsigned char *d) {
  int i;
  for (i=0; i<9; i++) {
    if ((d[0] & (0xff << (8-dlug_pfxbitlen[i]))) == dlug_pfx[i]) {
      return dlug_bytelen[i];
    }
  }
  return -2;
}

int dlug_fpeel(FILE *fp, unsigned char *buf) {
  unsigned char uc;
  int d, n, i;
  d = fgetc(fp);
  if (d==EOF) { return -1; }

  uc = (unsigned char)d;
  n = dlug_len(&uc);
  if (n<=0) { return -2; }

  buf[0] = uc;
  for (i=1; i<n; i++) {
    d = fgetc(fp);
    if (d==EOF) { return -1; }
    buf[i] = (unsigned char)d;
  }

  return n;
}

// Converts from uint8_t to dlug as a vector of unsigned chars
//
int dlug_append_uint8(std::vector<unsigned char> &v, uint8_t u) {
  if (u < (1<<dlug_bitlen[0])) {
    v.push_back((unsigned char)u);
    return 1;
  }

  v.push_back(0x80);
  v.push_back((unsigned char)u);
  return 2;

}

int dlug_append_uint32(std::vector<unsigned char> &v, uint32_t u) {
  if (u < (1<<dlug_bitlen[0])) {
    v.push_back((unsigned char)(u&0xff));
    return 1;
  }

  if (u < (1<<dlug_bitlen[1])) {
    v.push_back((unsigned char)((u>>8)&0xff) | dlug_pfx[1]);
    v.push_back((unsigned char)((u)&0xff));
    return 2;
  }

  if (u < (1<<dlug_bitlen[2])) {
    v.push_back((unsigned char)((u>>16)&0xff) | dlug_pfx[2]);
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 3;
  }

  if (u < (1<<dlug_bitlen[3])) {
    v.push_back((unsigned char)((u>>24)&0xff) | dlug_pfx[3]);
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 4;
  }

  v.push_back((unsigned char)dlug_pfx[4]);
  v.push_back((unsigned char)((u>>24)&0xff));
  v.push_back((unsigned char)((u>>16)&0xff));
  v.push_back((unsigned char)((u>>16)&0xff));
  v.push_back((unsigned char)((u>>8)&0xff));
  v.push_back((unsigned char)((u)&0xff));
  return 5;
}

int dlug_append_uint16(std::vector<unsigned char> &v, uint32_t u) {
  if (u < (1<<dlug_bitlen[0])) {
    v.push_back((unsigned char)(u&0xff));
    return 1;
  }

  if (u < (1<<dlug_bitlen[1])) {
    v.push_back((unsigned char)((u>>8)&0xff) | dlug_pfx[1]);
    v.push_back((unsigned char)((u)&0xff));
    return 2;
  }

  v.push_back((unsigned char)dlug_pfx[2]);
  v.push_back((unsigned char)((u>>8)&0xff));
  v.push_back((unsigned char)((u)&0xff));
  return 3;

}

int dlug_append_uint64(std::vector<unsigned char> &v, uint64_t u) {

  if (u < (1<<dlug_bitlen[0])) {
    v.push_back((unsigned char)(u&0xff));
    return 1;
  }

  if (u < (1<<dlug_bitlen[1])) {
    v.push_back((unsigned char)((u>>8)&0xff) | dlug_pfx[1]);
    v.push_back((unsigned char)((u)&0xff));
    return 2;
  }

  if (u < (1<<dlug_bitlen[2])) {
    v.push_back((unsigned char)((u>>16)&0xff) | dlug_pfx[2]);
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 3;
  }

  if (u < (1<<dlug_bitlen[3])) {
    v.push_back((unsigned char)((u>>24)&0xff) | dlug_pfx[3]);
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 4;
  }

  if (u < (1<<dlug_bitlen[4])) {
    v.push_back((unsigned char)((u>>32)&0xff) | dlug_pfx[4]);
    v.push_back((unsigned char)((u>>24)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 5;
  }

  if (u < (1<<dlug_bitlen[5])) {
    v.push_back((unsigned char)((u>>40)&0xff) | dlug_pfx[5]);
    v.push_back((unsigned char)((u>>32)&0xff));
    v.push_back((unsigned char)((u>>24)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 6;
  }

  if (u < (1<<dlug_bitlen[6])) {
    v.push_back((unsigned char)((u>>56)&0xff) | dlug_pfx[6]);
    v.push_back((unsigned char)((u>>48)&0xff));
    v.push_back((unsigned char)((u>>40)&0xff));
    v.push_back((unsigned char)((u>>32)&0xff));
    v.push_back((unsigned char)((u>>24)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>16)&0xff));
    v.push_back((unsigned char)((u>>8)&0xff));
    v.push_back((unsigned char)((u)&0xff));
    return 6;
  }


  v.push_back((unsigned char)dlug_pfx[7]);
  v.push_back((unsigned char)((u>>56)&0xff));
  v.push_back((unsigned char)((u>>48)&0xff));
  v.push_back((unsigned char)((u>>40)&0xff));
  v.push_back((unsigned char)((u>>32)&0xff));
  v.push_back((unsigned char)((u>>24)&0xff));
  v.push_back((unsigned char)((u>>16)&0xff));
  v.push_back((unsigned char)((u>>16)&0xff));
  v.push_back((unsigned char)((u>>8)&0xff));
  v.push_back((unsigned char)((u)&0xff));
  return 7;

}

int dlug_convert_uint8(unsigned char *d, uint8_t *u) {
  int idx;
  uint16_t u16=0;
  idx = dlug_index(d);
  if (idx<0) { return -1; }
  if (idx==0) {
    *u = d[0] & 0x7f;
    return 1;
  }

  if (dlug_bytelen[idx]>2) { return -1; }
  u16 = (((0xff >> dlug_pfxbitlen[idx]) & d[0]) << 8) + d[1];
  if (u16<=255) {
    *u = (uint8_t)u16;
    return 2;
  }

  return -1;
}

int dlug_convert_uint16(unsigned char *d, uint16_t *u) {
  int idx;
  uint32_t u32=0;
  idx = dlug_index(d);
  if (idx<0) { return -1; }
  if (idx==0) {
    *u = d[0] & (0xff >> dlug_pfxbitlen[idx]);
    return 1;
  }

  if (idx==1) {
    *u = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 8) + d[1];
    return 2;
  }

  if (idx==2) {
    u32 = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 16) + (d[1] << 8) + d[2];
    if (u32 <= (1<<16-1)) {
      *u = u32;
      return 3;
    }
  }

  return -1;
}

int dlug_convert_uint32(unsigned char *d, uint32_t *u) {
  int idx;
  uint32_t u64=0;
  idx = dlug_index(d);
  if (idx<0) { return -1; }
  if (idx==0) {
    *u = d[0] & (0xff >> dlug_pfxbitlen[idx]);
    return 1;
  }

  if (idx==1) {
    *u = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 8) + d[1];
    return 2;
  }

  if (idx==2) {
    *u = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 16) + (d[1] << 8) + d[2];
    return 3;
  }

  if (idx==3) {
    u64 = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 24) + (d[1] << 16) + (d[2] << 8) + d[3];
    if (u64 <= (1<<32-1)) {
      *u = u64;
      return 4;
    }
  }

  return -1;
}

int dlug_convert_uint64(unsigned char *d, uint64_t *u) {
  int idx;
  idx = dlug_index(d);
  if (idx<0) { return -1; }
  if (idx==0) {
    *u = d[0] & (0xff >> dlug_pfxbitlen[idx]);
    return 1;
  }

  if (idx==1) {
    *u = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 8) + d[1];
    return 2;
  }

  if (idx==2) {
    *u = ((d[0] & (0xff >> dlug_pfxbitlen[idx])) << 16) + (d[1] << 8) + d[2];
    return 3;
  }

  if (idx==3) {
    *u = ((d[0] & (0xffl >> dlug_pfxbitlen[idx])) << 24) + (d[1] << 16) + (d[2] << 8) + d[3];
    return 4;
  }

  if (idx==4) {
    *u = ((d[0] & (0xffl >> dlug_pfxbitlen[idx])) << 32) + (d[1] << 24) + (d[2] << 16) + (d[3] << 8) + d[4];
    return 5;
  }

  if (idx==5) {
    *u = ((d[0] & (0xffl >> dlug_pfxbitlen[idx])) << 40) + ((uint64_t)d[1] << 32) + (d[2] << 24) + (d[3] << 16) + (d[4] << 8) + d[5];
    return 6;
  }

  if (idx==6) {
    *u = ((d[0] & (0xffl >> dlug_pfxbitlen[idx])) << 48) + ((uint64_t)d[1] << 40) + ((uint64_t)d[2] << 32) + (d[3] << 24) + (d[4] << 16) + (d[5] << 8) + d[6];
    return 6;
  }

  if (idx==8) {
    *u = ((d[0] & (0xffl >> dlug_pfxbitlen[idx])) << 56) + ((uint64_t)d[1] << 48) + ((uint64_t)d[2] << 40) + ((uint64_t)d[3] << 32) + (d[4] << 24) + (d[5] << 16) + (d[6] << 8) + d[7];
    return 7;
  }

  return -1;
}

//int dlug_check(unsigned char *d) { return 0; }
//int dlug_checkcode(unsigned char *d) { return 0; }

int dlug_cmp(unsigned char *d0, unsigned char *d1) {
  int i;
  int d0_len, d1_len;
  int idx0, idx1, pfx0, pfx1;
  int nz0, nz1, remain_len0, remain_len1;
  int a, b;

  d0_len = dlug_len(d0);
  d1_len = dlug_len(d1);

  if ((d0_len<0) || (d1_len<0)) { return 0; }

  if (d0_len==d1_len) {
    idx0 = dlug_index(d0);
    if (idx0<0) { return 0; }

    pfx0 = d0[0] & (0xff >> dlug_pfxbitlen[idx0]);
    pfx1 = d1[0] & (0xff >> dlug_pfxbitlen[idx0]);

    for (i=1; i<d0_len; i++) {
      if (d0[i] < d1[i]) { return -1; }
      if (d0[i] > d1[i]) { return  1; }
    }

    return 0;
  }

  if      (d0_len < d1_len) { return -1; }
  else if (d0_len > d1_len) { return  1; }

  idx0 = dlug_index(d0);
  idx1 = dlug_index(d1);

  if ((idx0<0) || (idx1<0)) { return 0; }

  pfx0 = d0[0] & (0xff >> dlug_pfxbitlen[idx0]);
  pfx1 = d1[0] & (0xff >> dlug_pfxbitlen[idx1]);

  if (pfx0==0) {
    for (nz0=1; nz0<d0_len; nz0++) {
      if (d0[nz0] > 0) { break; }
    }
  }

  if (pfx1==0) {
    for (nz1=1; nz1<d1_len; nz1++) {
      if (d1[nz1] > 0) { break; }
    }
  }

  remain_len0 = d0_len - nz0;
  remain_len1 = d1_len - nz1;

  if (remain_len0 < remain_len1) { return -1; }
  if (remain_len0 > remain_len1) { return  1; }
  for (i=0; i<remain_len0; i++) {
    a = d0[nz0+i];
    if ((nz0+i)==0) { a = (0xff >> dlug_pfxbitlen[idx0]); }

    b = d1[nz1+i];
    if ((nz1+i)==0) { b = (0xff >> dlug_pfxbitlen[idx1]); }

    if (a<b) { return -1; }
    if (a>b) { return  1; }
  }

  return 0;
}


int dlug_test(void) {
  int i, j, k;
  uint8_t u8[] = {0,  127, 128, 255};
  uint16_t u16[] = {0, 127, 128, 1<<14 -1, 1<<14,1<<16-1};
  uint32_t u32[] = {0, 1<<7-1, 1<<7, 1<<14-1, 1<<14, 1<<27-1, 1<<27, 1l<<32-1};
  uint64_t u64[] = {0, 1<<7-1, 1<<7, 1<<14-1, 1<<14, 1<<27-1, 1<<27, 1l<<43-1, 1l<<43, 1l<<56-1, 1l<<56, 1l<<64-1};

  std::vector<unsigned char> v;

  printf("u8 tests:\n---\n");
  for (i=0; i<4; i++) {
    v.clear();
    dlug_append_uint8(v, u8[i]);

    for (k=0; k<v.size(); k++) {
      printf("[%d] %d (%x)\n", k, v[k], v[k]);
    }
    printf("\n");

  }
  printf("\n");

  printf("u16 tests:\n---\n");
  for (i=0; i<6; i++) {
    v.clear();
    dlug_append_uint16(v, u16[i]);

    for (k=0; k<v.size(); k++) {
      printf("[%d] %d (%x)\n", k, v[k], v[k]);
    }
    printf("\n");

  }
  printf("\n");

  printf("u32 tests:\n---\n");
  for (i=0; i<8; i++) {
    v.clear();
    k = dlug_append_uint32(v, u32[i]);

    printf(">> %d\n", k);
    for (k=0; k<v.size(); k++) {
      printf("[%d] %d (%x)\n", k, v[k], v[k]);
    }
    printf("\n");

  }
  printf("\n");

  printf("u64 tests:\n---\n");
  for (i=0; i<12; i++) {
    v.clear();
    k = dlug_append_uint64(v, u64[i]);

    printf(">> %d\n", k);
    for (k=0; k<v.size(); k++) {
      printf("[%d] %d (%x)\n", k, v[k], v[k]);
    }
    printf("\n");

  }
  printf("\n");

  return 0;


  unsigned char b0 = 0;
  unsigned char b1 = 1;
  unsigned char b2 = 8;
  unsigned char b3 = 64;
  unsigned char b4 = 127;

  uint16_t s0 = 128;
  uint16_t s1 = 129;

  unsigned char *buf;

  int x;

  printf(">>> %d %d\n", b0, dlug_index(&b0));
  printf(">>> %d %d\n", b1, dlug_index(&b1));
  printf(">>> %d %d\n", b2, dlug_index(&b2));
  printf(">>> %d %d\n", b3, dlug_index(&b3));
  printf(">>> %d %d\n", b4, dlug_index(&b4));

  x = (int)s0;
  buf = (unsigned char *)(&s0);
  printf(">>> %d %d\n", x, dlug_index(buf));

  x = (int)s1;
  buf = (unsigned char *)(&s1);
  printf(">>> %d %d\n", x, dlug_index(buf));
}

/*
int main(int argc, char **argv) {
  int i, j, k;

  std::vector<unsigned char> v;

  dlug_test();
  exit(0);

  dlug_append_uint32(v, 1<<27 - 1 );
  for (i=0; i<v.size(); i++) { printf("[%d] %d (%02x)\n", i, v[i], v[i]); }

  exit(0);

  for (i=0; i<8; i++) {
    printf("[%i] %i %i\n", i, dlug_bytelen[i], dlug_pfxbitlen[i]);
  }

  dlug_test();
}


*/
