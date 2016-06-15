#ifndef DLUG_H
#define DLUG_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>

#include <vector>

/*
//                       0 1 2 3 4 5 6 7 8
int dlug_bytelen[]    = {1,2,3,4,5,6,8,9,17};
int dlug_pfxbitlen[]  = {1,2,3,5,5,5,8,8,8};

//                           0   1    2    3    4    5    6    7   8
int dlug_bitlen[]         = {7,  14,  21,  27,  35,  43,  56,  64, 128};
unsigned char dlug_pfx[]  = {0,0x80,0xc0,0xe0,0xe8,0xf0,0xf8,0xf9,0xfa,0xff};
*/

extern int dlug_bytelen[];
extern int dlug_pfxbitlen[];

//                           0   1    2    3    4    5    6    7   8
extern int dlug_bitlen[];
extern unsigned char dlug_pfx[];

int dlug_index(unsigned char *d);
int dlug_len(unsigned char *d);
int dlug_fpeel(FILE *fp, unsigned char *buf);
int dlug_append_uint8(std::vector<unsigned char> &v, uint8_t u);
int dlug_append_uint32(std::vector<unsigned char> &v, uint32_t u);
int dlug_append_uint16(std::vector<unsigned char> &v, uint32_t u);
int dlug_append_uint64(std::vector<unsigned char> &v, uint64_t u);
int dlug_convert_uint8(unsigned char *d, uint8_t *u);
int dlug_convert_uint16(unsigned char *d, uint16_t *u);
int dlug_convert_uint32(unsigned char *d, uint32_t *u);
int dlug_convert_uint64(unsigned char *d, uint64_t *u);
int dlug_cmp(unsigned char *d0, unsigned char *d1);
int dlug_test(void);

#endif

