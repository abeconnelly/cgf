#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include "cgb.hpp"
#include "dlug.h"


int dlug_buf_peel(std::vector<char> &inp_buf, int inp_buf_pos, unsigned char *buf) {
  unsigned char uc;
  int d, n, i;

  d = inp_buf[inp_buf_pos++];

  uc = (unsigned char)d;
  n = dlug_len(&uc);
  if (n<=0) { return -2; }

  buf[0] = uc;
  for (i=1; i<n; i++) {
    d = inp_buf[inp_buf_pos++];
    buf[i] = (unsigned char)d;
  }

  return n;
}


inline void ull_from_c8(uint64_t *b, unsigned char *c8) {
  uint64_t t=0;
  uint64_t *p;

  /*
  t |= (uint64_t)((unsigned char)c8[0])<<0;
  t |= (uint64_t)((unsigned char)c8[1])<<8;
  t |= (uint64_t)((unsigned char)c8[2])<<16;
  t |= (uint64_t)((unsigned char)c8[3])<<24;
  t |= (uint64_t)((unsigned char)c8[4])<<32;
  t |= (uint64_t)((unsigned char)c8[5])<<40;
  t |= (uint64_t)((unsigned char)c8[6])<<48;
  t |= (uint64_t)((unsigned char)c8[7])<<56;
  */

  /*
  t |=
    ((uint64_t)((unsigned char)c8[0])<<0) |
    ((uint64_t)((unsigned char)c8[1])<<8) |
    ((uint64_t)((unsigned char)c8[2])<<16) |
    ((uint64_t)((unsigned char)c8[3])<<24) |
    ((uint64_t)((unsigned char)c8[4])<<32) |
    ((uint64_t)((unsigned char)c8[5])<<40) |
    ((uint64_t)((unsigned char)c8[6])<<48) |
    ((uint64_t)((unsigned char)c8[7])<<56);
    */

  p = (uint64_t *)c8;
  t = *p;

  *b = t;
}


int cgf_read_string(FILE *fp, cgf_string_t *s) {
  int i;
  int ch, n;

  ch = fgetc(fp);
  if (ch==EOF) { return EOF; }

  s->n = ch;
  s->s = (unsigned char *)malloc(sizeof(unsigned char)*(s->n+1));

  n = ch;

  for (i=0; i<n; i++) {
    ch = fgetc(fp);
    if (ch==EOF) { return EOF; }
    s->s[i] = (unsigned char)ch;
  }
  s->s[n] = '\0';

  //return s->s[n-1];
  return s->n + 1;
}

int cgf_read_string_buf(std::vector<char> &inp_buf, int inp_buf_pos, cgf_string_t *s) {
  int i;
  int ch, n;

  int orig_pos = inp_buf_pos;

  s->n = inp_buf[inp_buf_pos++];
  s->s = (unsigned char *)malloc(sizeof(unsigned char)*(s->n+1));

  n = s->n;

  for (i=0; i<n; i++) {
    s->s[i] = (unsigned char)inp_buf[inp_buf_pos++];
  }
  s->s[n] = '\0';

  return s->n + 1;
}

int cgf_read_uint64(FILE *fp, uint64_t *u) {
  int i, k;

  unsigned char buf[8];

  for (i=0; i<8; i++) {
    k = fgetc(fp);
    if (k==EOF) { return EOF; }
    buf[i] = (unsigned char)k;
  }

  ull_from_c8(u, buf);
  return 0;
}

inline int cgf_read_uint64_buf(std::vector<char> &inp_buf, int inp_buf_pos, uint64_t *u) {

  ull_from_c8(u, (unsigned char *)(&(inp_buf[inp_buf_pos])));
  return 8;


  //int i, k;
  unsigned char buf[8];
  //int orig_pos = inp_buf_pos;

  //for (i=0; i<8; i++) { buf[i] = (unsigned char)inp_buf[inp_buf_pos++]; }
  buf[0] = (unsigned char)inp_buf[inp_buf_pos+0];
  buf[1] = (unsigned char)inp_buf[inp_buf_pos+1];
  buf[2] = (unsigned char)inp_buf[inp_buf_pos+2];
  buf[3] = (unsigned char)inp_buf[inp_buf_pos+3];
  buf[4] = (unsigned char)inp_buf[inp_buf_pos+4];
  buf[5] = (unsigned char)inp_buf[inp_buf_pos+5];
  buf[6] = (unsigned char)inp_buf[inp_buf_pos+6];
  buf[7] = (unsigned char)inp_buf[inp_buf_pos+7];

  ull_from_c8(u, buf);
  //return inp_buf_pos - orig_pos;
  return 8;
}


int cgf_load_overflow(FILE *fp, cgf_overflow_t *ovf) {
  int i, j, k;
  int n, dn;
  uint64_t vec_n;
  int map_byte = 0;

  n=0;

  // Overflow
  cgf_read_uint64(fp, &(ovf->length));
  n+=8;

  cgf_read_uint64(fp, &(ovf->stride));
  n+=8;

  cgf_read_uint64(fp, &(ovf->map_byte_count));
  n+=8;


  vec_n = (ovf->length + ovf->stride - 1) / ovf->stride;
  ovf->offset = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
  ovf->position = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
  ovf->map = (uint8_t *)malloc(sizeof(uint8_t)*ovf->map_byte_count);

  for (i=0; i<vec_n; i++) {
    cgf_read_uint64(fp, &(ovf->offset[i]));
  }
  n+=8*vec_n;

  for (i=0; i<vec_n; i++) {
    cgf_read_uint64(fp, &(ovf->position[i]));
  }
  n+=8*vec_n;

  for (map_byte=0; map_byte < ovf->map_byte_count; map_byte++) {
    k = fgetc(fp);
    if (k==EOF) { return k; }
    ovf->map[map_byte] = (uint8_t)k;
    n++;
  }

  return n;
}

int cgf_load_overflow_buf(std::vector<char> &inp_buf, int inp_buf_pos, cgf_overflow_t *ovf) {
  int i, j, k;
  int n, dn;
  uint64_t vec_n;
  int map_byte = 0;

  int orig_pos = inp_buf_pos;

  n=0;

  // Overflow
  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(ovf->length));
  n+=dn;
  inp_buf_pos += dn;

  cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(ovf->stride));
  n+=dn;
  inp_buf_pos += dn;

  cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(ovf->map_byte_count));
  n+=dn;
  inp_buf_pos += dn;


  vec_n = (ovf->length + ovf->stride - 1) / ovf->stride;
  ovf->offset = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
  ovf->position = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
  ovf->map = (uint8_t *)malloc(sizeof(uint8_t)*ovf->map_byte_count);

  for (i=0; i<vec_n; i++) {
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(ovf->offset[i]));
    n += dn;
    inp_buf_pos += dn;
  }

  for (i=0; i<vec_n; i++) {
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(ovf->position[i]));
    n += dn;
    inp_buf_pos += dn;
  }

  /*
  for (map_byte=0; map_byte < ovf->map_byte_count; map_byte++) {
    k = inp_buf[inp_buf_pos++];
    ovf->map[map_byte] = (uint8_t)k;
    n++;
  }
  */
  for (map_byte=0; map_byte < ovf->map_byte_count; map_byte++) {
    ovf->map[map_byte] = (uint8_t)(inp_buf[inp_buf_pos+map_byte]);
  }
  n += ovf->map_byte_count;
  inp_buf_pos += ovf->map_byte_count;

  return n;
}

//-----

int cgf_load_final_overflow(FILE *fp, cgf_final_overflow_t *fin_ovf) {
  int i, j, k;
  uint64_t n_bytes;
  int map_byte = 0;
  int n=0;


  cgf_read_uint64(fp, &(fin_ovf->data_record_n));
  n+=8;

  cgf_read_uint64(fp, &(fin_ovf->data_record_byte_len));
  n+=8;

  fin_ovf->data_record = (cgf_data_record_t *)malloc(sizeof(cgf_data_record_t));

  fin_ovf->data_record->code = (uint8_t *)malloc(sizeof(uint8_t)*(fin_ovf->data_record_n));
  for (i=0; i<fin_ovf->data_record_n; i++) {
    k = fgetc(fp);
    if (k<0) { return k; }
    fin_ovf->data_record->code[i] = (uint8_t)k;
    n++;
  }

  n_bytes = fin_ovf->data_record_byte_len - fin_ovf->data_record_n;

  fin_ovf->data_record->data = (uint8_t *)malloc(sizeof(uint8_t)*(n_bytes));

  for (i=0; i<n_bytes; i++) {
    k = fgetc(fp);
    if (k<0) { return k; }
    fin_ovf->data_record->data[i] = (uint8_t)k;
    n++;
  }

  return n;
}

int cgf_load_final_overflow_buf(std::vector<char> &inp_buf, int inp_buf_pos, cgf_final_overflow_t *fin_ovf) {
  int i, j, k;
  uint64_t n_bytes;
  int map_byte = 0;
  int n=0, dn;

  int orig_pos = inp_buf_pos;


  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(fin_ovf->data_record_n));
  n+=dn;
  inp_buf_pos += dn;

  cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(fin_ovf->data_record_byte_len));
  n+=dn;
  inp_buf_pos += dn;

  fin_ovf->data_record = (cgf_data_record_t *)malloc(sizeof(cgf_data_record_t));

  fin_ovf->data_record->code = (uint8_t *)malloc(sizeof(uint8_t)*(fin_ovf->data_record_n));
  /*
  for (i=0; i<fin_ovf->data_record_n; i++) {
    k = inp_buf[inp_buf_pos++];
    fin_ovf->data_record->code[i] = (uint8_t)k;
    n++;
  }
  */
  for (i=0; i<fin_ovf->data_record_n; i++) {
    fin_ovf->data_record->code[i] = (uint8_t)(inp_buf[inp_buf_pos+i]);
  }
  n += fin_ovf->data_record_n;
  inp_buf_pos += fin_ovf->data_record_n;

  n_bytes = fin_ovf->data_record_byte_len - fin_ovf->data_record_n;

  fin_ovf->data_record->data = (uint8_t *)malloc(sizeof(uint8_t)*(n_bytes));

  /*
  for (i=0; i<n_bytes; i++) {
    k = inp_buf[inp_buf_pos++];
    fin_ovf->data_record->data[i] = (uint8_t)k;
    n++;
  }
  */
  for (i=0; i<n_bytes; i++) {
    fin_ovf->data_record->data[i] = (uint8_t)(inp_buf[inp_buf_pos+i]);
  }
  n += n_bytes;
  inp_buf_pos += n_bytes;

  return n;
}

//-----

int cgf_load_low_quality_info(FILE *fp, cgf_low_quality_info_t *loq_info) {
  int i, j, k;
  uint64_t vec_n, index_length;
  int map_byte = 0;
  int n=0;


  cgf_read_uint64(fp, &(loq_info->count));
  n+=8;

  cgf_read_uint64(fp, &(loq_info->code));
  n+=8;

  cgf_read_uint64(fp, &(loq_info->stride));
  n+=8;

  index_length = (loq_info->count + loq_info->stride - 1 )/ loq_info->stride;

  loq_info->offset = (uint64_t *)malloc(sizeof(uint64_t)*(index_length));
  for (i=0; i<index_length; i++) {
    cgf_read_uint64(fp, &(loq_info->offset[i]));
    n+=8;
  }

  //--

  loq_info->step_position = (uint64_t *)malloc(sizeof(uint64_t)*(index_length));
  for (i=0; i<index_length; i++) {
    cgf_read_uint64(fp, &(loq_info->step_position[i]));
    n+=8;
  }

  vec_n = (loq_info->count + 7) / 8;

  loq_info->hom_flag = (uint8_t *)malloc(sizeof(uint8_t)*vec_n);
  for (i=0; i<vec_n; i++) {
    k = fgetc(fp);
    if (k<0) { return k; }
    loq_info->hom_flag[i] = k;
    n++;
  }

  //--

  cgf_read_uint64(fp, &(loq_info->loq_flag_byte_count));
  n+=8;

  loq_info->loq_flag = (uint8_t *)malloc(sizeof(uint8_t)*(loq_info->loq_flag_byte_count));
  for (i=0; i<loq_info->loq_flag_byte_count; i++) {
    k = fgetc(fp);
    if (k<0) { return k; }
    loq_info->loq_flag[i] = k;
    n++;
  }

  //--

  cgf_read_uint64(fp, &(loq_info->loq_info_byte_count));
  n+=8;

  loq_info->loq_info = (uint8_t *)malloc(sizeof(uint8_t)*(loq_info->loq_info_byte_count));
  for (i=0; i<loq_info->loq_info_byte_count; i++) {
    k = fgetc(fp);
    if (k<0) { return k; }
    loq_info->loq_info[i] = k;
    n++;
  }

  return n;
}


int cgf_load_low_quality_info_buf(std::vector<char> &inp_buf, int inp_buf_pos, cgf_low_quality_info_t *loq_info) {
  int i, j, k;
  uint64_t vec_n, index_length;
  int map_byte = 0;
  int n=0, dn;


  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->count));
  n+=dn;
  inp_buf_pos += dn;

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->code));
  n+=dn;
  inp_buf_pos += dn;

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->stride));
  n+=dn;
  inp_buf_pos += dn;


  index_length = (loq_info->count + loq_info->stride - 1 )/ loq_info->stride;

  loq_info->offset = (uint64_t *)malloc(sizeof(uint64_t)*(index_length));
  for (i=0; i<index_length; i++) {
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->offset[i]));
    n+=dn;
    inp_buf_pos += dn;
  }

  //--

  loq_info->step_position = (uint64_t *)malloc(sizeof(uint64_t)*(index_length));
  for (i=0; i<index_length; i++) {
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->step_position[i]));
    n+=dn;
    inp_buf_pos += dn;
  }

  vec_n = (loq_info->count + 7) / 8;

  loq_info->hom_flag = (uint8_t *)malloc(sizeof(uint8_t)*vec_n);
  /*
  for (i=0; i<vec_n; i++) {
    k = inp_buf[inp_buf_pos++];
    loq_info->hom_flag[i] = k;
    n++;
  }
  */
  for (i=0; i<vec_n; i++) { loq_info->hom_flag[i] = inp_buf[inp_buf_pos+i]; }
  //memcpy(loq_info->hom_flag, &(inp_buf[inp_buf_pos]), sizeof(char)*vec_n);
  n += vec_n;
  inp_buf_pos += vec_n;

  //--

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->loq_flag_byte_count));
  n+=dn;
  inp_buf_pos += dn;

  loq_info->loq_flag = (uint8_t *)malloc(sizeof(uint8_t)*(loq_info->loq_flag_byte_count));
  /*
  for (i=0; i<loq_info->loq_flag_byte_count; i++) {
    k = inp_buf[inp_buf_pos++];
    loq_info->loq_flag[i] = k;
    n++;
  }
  */
  for (i=0; i<loq_info->loq_flag_byte_count; i++) { loq_info->loq_flag[i] = inp_buf[inp_buf_pos+i]; }
  //memcpy(loq_info->loq_flag, &(inp_buf[inp_buf_pos]), loq_info->loq_flag_byte_count);
  n += loq_info->loq_flag_byte_count;
  inp_buf_pos += loq_info->loq_flag_byte_count;

  //--

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(loq_info->loq_info_byte_count));
  n+=dn;
  inp_buf_pos += dn;

  loq_info->loq_info = (uint8_t *)malloc(sizeof(uint8_t)*(loq_info->loq_info_byte_count));
  /*
  for (i=0; i<loq_info->loq_info_byte_count; i++) {
    k = inp_buf[inp_buf_pos++];
    loq_info->loq_info[i] = k;
    n++;
  }
  */
  for (i=0; i<loq_info->loq_info_byte_count; i++) { loq_info->loq_info[i] = inp_buf[inp_buf_pos+i]; }
  //memcpy(loq_info->loq_info, &(inp_buf[inp_buf_pos]), sizeof(char)*(loq_info->loq_info_byte_count));
  n += loq_info->loq_info_byte_count;
  inp_buf_pos += loq_info->loq_info_byte_count;

  return n;
}


//-----

cgf_t *load_cgf_buf(FILE *fp) {
  int i, k;
  cgf_t *cgf=NULL;
  uint64_t b;
  unsigned char buf[8];
  uint8_t u8;
  unsigned char c;
  int n, ch;
  int dn;

  uint32_t u32;

  char magic[9] = "\"cgf.b\"{";

  std::vector<char> inp_buf;
  char *tbuf=NULL;
  int n_buf = 4096;
  ssize_t tmp_sz;
  ssize_t sz, tot_sz=0;
  int inp_buf_pos=0;

  cgf_path_t *path;
  cgf_overflow_t *ovf;
  cgf_final_overflow_t *fin_ovf;
  cgf_final_overflow_map_opt_t *opt_ovf;
  cgf_low_quality_info_t *loq_info;

  int tilepath;
  uint64_t byte_offset, vec_n;

  tbuf = (char *)malloc(sizeof(char)*n_buf);

  inp_buf.reserve((1024*1024*40));

  do {
    sz = fread(tbuf, sizeof(char), n_buf, fp);
    inp_buf.insert(inp_buf.end(), tbuf, tbuf+sz);
    tot_sz += sz;
  } while (sz==n_buf);

  if (!feof(fp)) { goto load_cgf_cleanup; }


  for (i=0; i<8; i++) {
    buf[i] = inp_buf[inp_buf_pos++];
    if (buf[i]==EOF) { goto load_cgf_cleanup; }
    if (buf[i]!=magic[i]) { goto load_cgf_cleanup; }
  }

  cgf = (cgf_t *)malloc(sizeof(cgf_t));
  ull_from_c8(&(cgf->magic), buf);

  dn = cgf_read_string_buf(inp_buf, inp_buf_pos, &(cgf->cgf_version));
  inp_buf_pos += dn;

  dn = cgf_read_string_buf(inp_buf, inp_buf_pos, &(cgf->lib_version));
  inp_buf_pos += dn;

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(cgf->path_count));
  inp_buf_pos += dn;

  dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(cgf->tile_map_len));
  inp_buf_pos += dn;

  // Tile Map
  //
  cgf->tile_map_bytes = (unsigned char *)malloc(sizeof(unsigned char)*(cgf->tile_map_len));

  n = 0;
  while (n<cgf->tile_map_len) {

    //dn = dlug_fpeel(fp, cgf->tile_map_bytes + n);
    dn = dlug_buf_peel(inp_buf, inp_buf_pos, cgf->tile_map_bytes + n);
    if (dn<0) {
      printf("error: got dn %d bytes read while reading tile_map\n", dn);
      goto load_cgf_cleanup;
    }
    n += dn;
    inp_buf_pos += dn;

  }

  cgf_unpack_tile_map(cgf);

  // Step Per Path
  //
  cgf->step_per_path = (uint64_t *)malloc(sizeof(uint64_t)*(cgf->path_count));
  for (i=0; i<cgf->path_count; i++) {
    //k = cgf_read_uint64(fp, &(cgf->step_per_path[i]));
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(cgf->step_per_path[i]));
    if (dn<=0) { goto load_cgf_cleanup; }
    inp_buf_pos += dn;
  }

  // Path Struct Offset
  //
  cgf->path_struct_offset = (uint64_t *)malloc(sizeof(uint64_t)*(cgf->path_count+1));
  for (i=0; i<=cgf->path_count; i++) {
    //k = cgf_read_uint64(fp, &(cgf->path_struct_offset[i]));
    dn = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(cgf->path_struct_offset[i]));
    if (dn<=0) { goto load_cgf_cleanup; }
    inp_buf_pos += dn;
  }

  byte_offset = 0;

  cgf->path = (cgf_path_t *)malloc(sizeof(cgf_path_t)*(cgf->path_count));
  for (tilepath=0; tilepath<cgf->path_count; tilepath++) {
    path = &(cgf->path[tilepath]);
    path->overflow            = (cgf_overflow_t *)malloc(sizeof(cgf_overflow_t));
    path->final_overflow      = (cgf_final_overflow_t *)malloc(sizeof(cgf_final_overflow_t));
    path->final_overflow_opt  = (cgf_final_overflow_map_opt_t *)malloc(sizeof(cgf_final_overflow_map_opt_t));
    path->loq_info            = (cgf_low_quality_info_t *)malloc(sizeof(cgf_low_quality_info_t));

    k = cgf_read_string_buf(inp_buf, inp_buf_pos, &(path->name));
    if (k<=0) { goto load_cgf_cleanup; }
    byte_offset += k;
    inp_buf_pos += k;

    k = cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(path->n_tile));
    if (k<=0) { goto load_cgf_cleanup; }
    byte_offset += 8;
    inp_buf_pos += k;

    vec_n = (path->n_tile+31)/32;

    path->vec = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
    for (i=0; i<vec_n; i++) {
      cgf_read_uint64_buf(inp_buf, inp_buf_pos, &(path->vec[i]));
      byte_offset+=8;
      inp_buf_pos += 8;
    }

    //--

    k = cgf_load_overflow_buf(inp_buf, inp_buf_pos, path->overflow);
    if (k<=0) {
      fprintf(stderr, "EOF encountered when loading overflow\n");
      goto load_cgf_cleanup;
    }
    byte_offset += k;
    inp_buf_pos += k;

    //--

    k = cgf_load_final_overflow_buf(inp_buf, inp_buf_pos, path->final_overflow);
    if (k<=0) {
      fprintf(stderr, "EOF encountered when loading overflow\n");
      goto load_cgf_cleanup;
    }
    byte_offset += k;
    inp_buf_pos += k;


    //--
    // The 'opt' final overflow is vestigial and unused
    //

    k = cgf_load_low_quality_info_buf(inp_buf, inp_buf_pos, path->loq_info);
    if (k<=0) {
      fprintf(stderr, "EOF encountered when loading overflow (%d)\n", k);
      goto load_cgf_cleanup;
    }
    byte_offset += k;
    inp_buf_pos += k;

    if (byte_offset != cgf->path_struct_offset[tilepath+1]) {
      fprintf(stderr, "ERROR: byte offset mismatch for tilepath %d (%d != %d)\n",
          tilepath,
          (int)byte_offset, (int)cgf->path_struct_offset[tilepath+1]
          );
      goto load_cgf_cleanup;
    }

  }

  free(tbuf);

  return cgf;

load_cgf_cleanup:

  free(tbuf);

  if (cgf) { free(cgf); }
  return NULL;
}

cgf_t *load_cgf(FILE *fp) {
  int i, k;
  cgf_t *cgf=NULL;
  uint64_t b;
  unsigned char buf[8];
  uint8_t u8;
  unsigned char c;
  int n, ch;
  int dn;

  uint32_t u32;

  char magic[9] = "\"cgf.b\"{";

  cgf_path_t *path;
  cgf_overflow_t *ovf;
  cgf_final_overflow_t *fin_ovf;
  cgf_final_overflow_map_opt_t *opt_ovf;
  cgf_low_quality_info_t *loq_info;

  int tilepath;
  uint64_t byte_offset, vec_n;

  for (i=0; i<8; i++) {
    buf[i] = fgetc(fp);
    if (buf[i]==EOF) { goto load_cgf_cleanup; }
    if (buf[i]!=magic[i]) { goto load_cgf_cleanup; }
  }

  cgf = (cgf_t *)malloc(sizeof(cgf_t));
  ull_from_c8(&(cgf->magic), buf);

  cgf_read_string(fp, &(cgf->cgf_version));
  cgf_read_string(fp, &(cgf->lib_version));

  cgf_read_uint64(fp, &(cgf->path_count));
  cgf_read_uint64(fp, &(cgf->tile_map_len));

  // Tile Map
  //
  cgf->tile_map_bytes = (unsigned char *)malloc(sizeof(unsigned char)*(cgf->tile_map_len));

  n = 0;
  while (n<cgf->tile_map_len) {

    dn = dlug_fpeel(fp, cgf->tile_map_bytes + n);
    if (dn<0) {
      printf("error: got dn %d bytes read while reading tile_map\n", dn);
      goto load_cgf_cleanup;
    }
    n += dn;

  }

  cgf_unpack_tile_map(cgf);

  // Step Per Path
  //
  cgf->step_per_path = (uint64_t *)malloc(sizeof(uint64_t)*(cgf->path_count));
  for (i=0; i<cgf->path_count; i++) {
    k = cgf_read_uint64(fp, &(cgf->step_per_path[i]));
    if (k==EOF) { goto load_cgf_cleanup; }
  }

  // Path Struct Offset
  //
  cgf->path_struct_offset = (uint64_t *)malloc(sizeof(uint64_t)*(cgf->path_count+1));
  for (i=0; i<=cgf->path_count; i++) {
    k = cgf_read_uint64(fp, &(cgf->path_struct_offset[i]));
    if (k==EOF) { goto load_cgf_cleanup; }
  }

  byte_offset = 0;

  cgf->path = (cgf_path_t *)malloc(sizeof(cgf_path_t)*(cgf->path_count));
  for (tilepath=0; tilepath<cgf->path_count; tilepath++) {
    path = &(cgf->path[tilepath]);
    path->overflow            = (cgf_overflow_t *)malloc(sizeof(cgf_overflow_t));
    path->final_overflow      = (cgf_final_overflow_t *)malloc(sizeof(cgf_final_overflow_t));
    path->final_overflow_opt  = (cgf_final_overflow_map_opt_t *)malloc(sizeof(cgf_final_overflow_map_opt_t));
    path->loq_info            = (cgf_low_quality_info_t *)malloc(sizeof(cgf_low_quality_info_t));

    k = cgf_read_string(fp, &(path->name));
    if (k==EOF) { goto load_cgf_cleanup; }
    byte_offset += k;

    k = cgf_read_uint64(fp, &(path->n_tile));
    if (k==EOF) { goto load_cgf_cleanup; }
    byte_offset += 8;

    vec_n = (path->n_tile+31)/32;
    path->vec = (uint64_t *)malloc(sizeof(uint64_t)*vec_n);
    for (i=0; i<vec_n; i++) {
      cgf_read_uint64(fp, &(path->vec[i]));
      byte_offset+=8;
    }

    //--

    k = cgf_load_overflow(fp, path->overflow);
    if (k==EOF) {
      fprintf(stderr, "EOF encountered when loading overflow\n");
      goto load_cgf_cleanup;
    }
    byte_offset += k;

    //--

    k = cgf_load_final_overflow(fp, path->final_overflow);
    if (k==EOF) {
      fprintf(stderr, "EOF encountered when loading overflow\n");
      goto load_cgf_cleanup;
    }
    byte_offset += k;


    //--
    // The 'opt' final overflow is vestigial and unused
    //

    k = cgf_load_low_quality_info(fp, path->loq_info);
    if (k<0) {
      fprintf(stderr, "EOF encountered when loading overflow (%d)\n", k);
      goto load_cgf_cleanup;
    }
    byte_offset += k;

    if (byte_offset != cgf->path_struct_offset[tilepath+1]) {
      fprintf(stderr, "ERROR: byte offset mismatch for tilepath %d (%d != %d)\n",
          tilepath,
          (int)byte_offset, (int)cgf->path_struct_offset[tilepath+1]
          );
      goto load_cgf_cleanup;
    }

  }


  return cgf;

load_cgf_cleanup:

  if (cgf) { free(cgf); }
  return NULL;
}

cgf_t *load_cgf_fn(const char *fn) {
  FILE *fp;
  cgf_t *cgf;
  if (!(fp = fopen(fn, "r"))) { return NULL; }
  //cgf = load_cgf(fp);
  cgf = load_cgf_buf(fp);
  fclose(fp);
  return cgf;
}



