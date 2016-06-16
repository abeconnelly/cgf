#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include "cgb.hpp"
#include "dlug.h"

void show_help() {
  printf("\n");
  printf("compact genome tool\n");
  printf("\n");
  printf("usage:\n");
  printf("  -i cgf        input cgf file\n");
  printf("  [-D]          debug print\n");
  printf("  [-S]          stats print\n");
  printf("  [-h]          help\n");
  printf("\n");
}

void ull_from_c8(uint64_t *b, unsigned char *c8) {
  uint64_t t=0;

  t |= (uint64_t)((unsigned char)c8[0])<<0;
  t |= (uint64_t)((unsigned char)c8[1])<<8;
  t |= (uint64_t)((unsigned char)c8[2])<<16;
  t |= (uint64_t)((unsigned char)c8[3])<<24;
  t |= (uint64_t)((unsigned char)c8[4])<<32;
  t |= (uint64_t)((unsigned char)c8[5])<<40;
  t |= (uint64_t)((unsigned char)c8[6])<<48;
  t |= (uint64_t)((unsigned char)c8[7])<<56;

  *b = t;
}

int cgf_read_dlug(FILE *fp, unsigned char *buf) {
  int dn = 0;

  return -1;
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

int cgf_print_tile_map(cgf_t *cgf) {
  int i, j, k;
  char allele[] = "ab";
  int pmod=32;

  printf("TileMap:");
  for (i=0; i<cgf->n_tile_map; i++) {
    if ((i%pmod)==0) {
      if (i>0) { printf(","); }
      printf("\n  ");
    } else {
      printf(",");
    }
    printf(" [");
    for (j=0; j<2; j++) {
      if (j>0) { printf(","); }
      printf("[");
      for (k=0; k<cgf->tile_map[i][j][0]; k++) {
        if (k>0) { printf(","); }
        printf("%x+%x", cgf->tile_map[i][j][2*k+1],  cgf->tile_map[i][j][2*k+2]);
      }
      printf("]");
    }
    printf("]");
  }
  printf("\n");

  return 0;
}

int cgf_unpack_tile_map(cgf_t *cgf) {
  unsigned char *b;

  int n, dn, N;
  uint32_t n0, n1, v, s, m[2];
  int i, j, k, cur=0;

  N = cgf->tile_map_len;
  b = cgf->tile_map_bytes;

  n = 0;
  while (n<N) {

    dn = dlug_convert_uint32(b + n, &(m[0]));
    if (dn<0) { return -1; }
    n+=dn;

    dn = dlug_convert_uint32(b + n, &(m[1]));
    if (dn<0) { return -1; }
    n+=dn;

    cur++;

    for (j=0; j<2; j++) {
      for (i=0; i<m[j]; i++) {
        dn = dlug_convert_uint32(b + n, &v);
        if (dn<0) { return -1; }
        n+=dn;

        dn = dlug_convert_uint32(b + n, &s);
        if (dn<0) { return -1; }
        n+=dn;
      }
    }
  }

  cgf->n_tile_map = cur;


  cgf->tile_map = (int ***)malloc(sizeof(int **)*cur);
  n = 0;
  cur=0;
  while (n<N) {

    dn = dlug_convert_uint32(b + n, &(m[0]));
    if (dn<0) { return -1; }
    n+=dn;

    dn = dlug_convert_uint32(b + n, &(m[1]));
    if (dn<0) { return -1; }
    n+=dn;

    cgf->tile_map[cur] = (int **)malloc(sizeof(int *)*2);

    cgf->tile_map[cur][0] = (int *)malloc(sizeof(int)*(2*m[0]+1));
    cgf->tile_map[cur][1] = (int *)malloc(sizeof(int)*(2*m[1]+1));

    cgf->tile_map[cur][0][0] = m[0];
    cgf->tile_map[cur][1][0] = m[1];

    for (j=0; j<2; j++) {
      for (i=0; i<m[j]; i++) {
        dn = dlug_convert_uint32(b + n, &v);
        if (dn<0) { return -1; }
        n+=dn;


        dn = dlug_convert_uint32(b + n, &s);
        if (dn<0) { return -1; }
        n+=dn;

        cgf->tile_map[cur][j][2*i+1] = v;
        cgf->tile_map[cur][j][2*i+2] = s;
      }
    }

    cur++;
  }

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

void cgf_print_overflow(cgf_overflow_t *ovf, int tilepath) {
  int i, j, k;
  uint64_t vec_n;
  int n, dn;
  int map_byte = 0;
  int pos;
  uint32_t u;

  vec_n = (ovf->length + ovf->stride - 1) / ovf->stride;

  printf("  %04x.Overlfow.Length: %i\n", tilepath, (int)ovf->length);
  printf("  %04x.Overflow.Stride: %i\n", tilepath, (int)ovf->stride);
  printf("  %04x.Overflow.MapCountByte: %i\n", tilepath, (int)ovf->map_byte_count);

  printf("  %04x.Overflow.Offset[%d]:", tilepath, (int)vec_n);
  for (i=0; i<vec_n; i++) {
    printf(" %llu", (unsigned long long int)(ovf->offset[i]));
  }
  printf("\n");

  printf("  %04x.Overflow.Position[%d]:", tilepath, (int)vec_n);
  for (i=0; i<vec_n; i++) {
    printf(" %llu", (unsigned long long int)(ovf->position[i]));
  }
  printf("\n");

  printf("  %04x.Overflow.Map[%d]:", tilepath, (int)ovf->map_byte_count);
  pos = 0;
  for (map_byte=0; map_byte<ovf->map_byte_count; ) {
    dn = dlug_convert_uint32(ovf->map + map_byte, &u);
    if (dn<=0) { return ; }
    map_byte += dn;

    if ((pos%64)==0) {
      printf("\n    ");
    }
    printf(" %x", (int)u);
    pos++;

  }
  printf("\n");

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

void cgf_print_final_overflow(cgf_final_overflow_t *fin_ovf, int tilepath) {
  int i, j, k, n=0, dn, record = 0;
  uint64_t N;
  uint32_t u, n_allele, a_len, vid, span;

  printf("  %04x.FinalOverflow.DataRecordN: %llu\n", tilepath, (unsigned long long int)fin_ovf->data_record_n);
  printf("  %04x.FinalOverflow.DataRecordByteLen: %llu\n", tilepath, (unsigned long long int)fin_ovf->data_record_byte_len);

  N = fin_ovf->data_record_n;
  printf("  %04x.FinalOverflow.DataRecord.Code:", tilepath);
  for (i=0; i<N; i++) {
    if ((i%32)==0) { printf("\n    "); }
    printf(" %x", (int)fin_ovf->data_record->code[i]);
  }
  printf("\n");

  N = fin_ovf->data_record_byte_len - fin_ovf->data_record_n;
  printf("  %04x.FinalOverflow.DataRecord.Data:\n", tilepath);
  for (record=0,n=0; n<N; record++) {
    dn = dlug_convert_uint32(fin_ovf->data_record->data + n, &u);
    if (dn<=0) { return; }
    n+=dn;

    printf("    [%i] %04x.%04x: ", record, tilepath, u);

    dn = dlug_convert_uint32(fin_ovf->data_record->data + n, &n_allele);
    if (dn<=0) { return; }
    n+=dn;

    printf("[");
    for (i=0; i<n_allele; i++) {

      dn = dlug_convert_uint32(fin_ovf->data_record->data + n, &a_len);
      if (dn<=0) { return; }
      n+=dn;

      if (i>0) { printf(","); }
      printf("[");

      for (j=0; j<a_len; j++) {

        if (j>0) { printf(","); }

        dn = dlug_convert_uint32(fin_ovf->data_record->data + n, &vid);
        if (dn<=0) { return; }
        n+=dn;

        printf(" %x", vid);

        dn = dlug_convert_uint32(fin_ovf->data_record->data + n, &span);
        if (dn<=0) { return; }
        n+=dn;

        printf("+%x", span);
      }
      printf(" ]");
    }
    printf("]\n");
  }

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

int cgf_print_low_quality_info(cgf_low_quality_info_t *loq_info, int tilepath) {
  int i, j, k;
  uint64_t vec_n, index_length;
  int map_byte = 0;
  int n=0;

  printf("  %04x.Loq.Count: %llu\n", tilepath, (unsigned long long int)loq_info->count);
  printf("  %04x.Loq.Code: %llu\n", tilepath, (unsigned long long int)loq_info->code);
  printf("  %04x.Loq.Stride: %llu\n", tilepath, (unsigned long long int)loq_info->stride);

  vec_n = (loq_info->count + loq_info->stride - 1 )/ loq_info->stride;

  printf("  %04x.Loq.Offset[%d]:", tilepath, (int)vec_n);
  for (i=0; i<vec_n; i++) {
    printf(" %llu", (unsigned long long int)loq_info->offset[i]);
  }
  printf("\n");

  printf("  %04x.Loq.TilePosition[%d]:", tilepath, (int)(vec_n+1));
  for (i=0; i<vec_n; i++) {
    printf(" %llu", (unsigned long long int)loq_info->step_position[i]);
  }
  printf("\n");

  vec_n = (loq_info->count + 7) / 8;

  printf("  %04x.Loq.HomFlag[%d]:", tilepath, (int)(vec_n));
  for (i=0; i<vec_n; i++) {
    if ((i%64)==0) { printf("\n    "); }
    printf(" %02x", (unsigned char)loq_info->hom_flag[i]);
  }
  printf("\n");

  printf("\n");

  printf("  %04x.Loq.LoqFlagByteCount: %d\n", tilepath, (int)(loq_info->loq_flag_byte_count));
  printf("  %04x.Loq.LoqFlag[%d]:", tilepath, (int)(loq_info->loq_flag_byte_count));
  for (i=0; i<loq_info->loq_flag_byte_count; i++) {
    if ((i%64)==0) { printf("\n    "); }
    printf(" %02x", (unsigned char)loq_info->loq_flag[i]);
  }
  printf("\n");

  printf("\n");

  printf("  %04x.Loq.LoqInfoByteCount: %d\n", tilepath, (int)(loq_info->loq_info_byte_count));
  printf("  %04x.Loq.LoqInfo[%d]:", tilepath, (int)(loq_info->loq_info_byte_count));
  for (i=0; i<loq_info->loq_info_byte_count; i++) {
    if ((i%64)==0) { printf("\n    "); }
    printf(" %02x", (unsigned char)loq_info->loq_info[i]);
  }
  printf("\n");

}


//-----

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

void debug_print_cgf(cgf_t *cgf) {
  int i, j, k;
  unsigned char c;
  cgf_path_t *path;
  int pmod = 32;
  int tilepath;
  uint64_t vec_n;

  int print_overflow = 1, print_final_overflow=1, print_loq = 1;

  //--------------------------------
  // print debug
  //--------------------------------


  printf("Magic: ");
  for (i=0; i<8; i++) {
    c = (unsigned char)((cgf->magic >> (i*8)) & 0xff);
    printf("%c", c);
  }
  printf(" (%08" PRIx64 ")\n", cgf->magic);

  printf("CGFVersion: %s\n", cgf->cgf_version.s);
  printf("LibVersion: %s\n", cgf->lib_version.s);

  printf("PathCount: %" PRId64  "\n", cgf->path_count);
  printf("TileMapLength: %" PRId64  "\n", cgf->tile_map_len);

  cgf_print_tile_map(cgf);


  printf("StepPerPath:");
  for (i=0; i<cgf->path_count; i++) {
    if ((i%pmod)==0) {
      printf("\n    ");
    }
    printf(" %04llx", (unsigned long long int)(cgf->step_per_path[i]));
  }
  printf("\n");

  pmod=16;
  printf("PathStructOffset:");
  for (i=0; i<=cgf->path_count; i++) {
    if ((i%pmod)==0) {
      printf("\n    ");
    }
    printf(" %08llx", (unsigned long long int)(cgf->path_struct_offset[i]));
  }
  printf("\n");

  for (tilepath=0; tilepath<cgf->path_count; tilepath++) {
    path = &(cgf->path[tilepath]);

    printf("  %04x.Name: %s\n", tilepath, path->name.s);
    printf("  %04x.NTile: %d\n", tilepath, (int)path->n_tile);



    vec_n = (path->n_tile+31)/32;
    printf("  %04x.Vec[%d]:", tilepath, (int)vec_n);
    for (i=0; i<vec_n; i++) {
      if ((i%8)==0) { printf("\n    "); }
      printf(" %016llx", (unsigned long long int)path->vec[i]);

    }
    printf("\n");

    if (print_overflow) {
      cgf_print_overflow(path->overflow, tilepath);
    }

    if (print_final_overflow) {
      cgf_print_final_overflow(path->final_overflow, tilepath);
    }

    if (print_loq) {
      cgf_print_low_quality_info(path->loq_info, tilepath);
    }
  }


}

void stats_print_cgf(cgf_t *cgf) {
  uint64_t vec_bytes=0, ovf_bytes=0, fin_ovf_bytes=0, loq_bytes=0;
  int i, j, k;
  int tilepath;
  cgf_path_t *path;
  cgf_overflow_t *ovf;
  cgf_final_overflow_t *fin_ovf;
  cgf_low_quality_info_t *loq;
  uint64_t n_bytes, n_vec;

  uint64_t tot_tile=0;

  for (tilepath=0; tilepath<cgf->path_count; tilepath++) {
    path = &(cgf->path[tilepath]);
    ovf     = path->overflow;
    fin_ovf = path->final_overflow;
    loq     = path->loq_info;

    vec_bytes += 8*((path->n_tile + 31) / 32);

    tot_tile += path->n_tile;

    n_vec = (ovf->length + ovf->stride - 1) / ovf->stride;
    ovf_bytes += 8 + 8 + 8 + 2*(8*n_vec) + ovf->map_byte_count;

    fin_ovf_bytes += 8 + 8 + fin_ovf->data_record_byte_len;

    n_vec = (loq->count + loq->stride - 1 )/ loq->stride;
    loq_bytes += 8 + 8 + 2*(8*n_vec) + (n_vec/8) + 8 + loq->loq_flag_byte_count + 8 + loq->loq_info_byte_count;
  }

  printf("Total Tiles: %llu (%dM)\n", (unsigned long long int)tot_tile, (int)(tot_tile/(1000*1000)));
  printf("VecBytes: %llu (%dMiB)\n", (unsigned long long int)vec_bytes, (int)(vec_bytes/(1024*1024)));
  printf("Overflow: %llu (%dMib)\n", (unsigned long long int)ovf_bytes, (int)(ovf_bytes/(1024*1024)));
  printf("FinalOverflow: %llu (%dMiB)\n", (unsigned long long int)fin_ovf_bytes, (int)(fin_ovf_bytes/(1024*1024)));
  printf("Loq: %llu (%dMiB)\n", (unsigned long long int)loq_bytes, (int)(loq_bytes/(1024*1024)));

}

cgf_t *load_cgf_fn(const char *fn) {
  FILE *fp;
  cgf_t *cgf;
  if (!(fp = fopen(fn, "r"))) { return NULL; }
  cgf = load_cgf(fp);
  fclose(fp);
  return cgf;
}


// http://stackoverflow.com/questions/109023/how-to-count-the-number-of-set-bits-in-a-32-bit-integer
//
// Based on 'divide and merge'.  e.g.
//
// unsigned int count_bit(unsigned int x)
// {
//   x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
//   x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
//   x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
//   x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
//   x = (x & 0x0000FFFF) + ((x >> 16)& 0x0000FFFF);
//   return x;
// }
//
int NumberOfSetBits(uint32_t u)
{
  u = u - ((u >> 1) & 0x55555555);
  u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
  return (((u + (u >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}


// Only check the canonical bits for matches within the
// [start_step,start_step+n_step) range.
//
// Store matched results in 'n_match'.
//
int cgf_tile_concordance_0(int *n_match,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath, int start_step, int n_step) {

  int i, j, k;
  int start_block, end_block, s;
  cgf_path_t *path_a, *path_b;
  uint64_t mask, z;
  uint32_t u32, x32, y32;

  uint64_t start_mask, end_mask;
  int canonical_count=0;

  path_a = &(cgf_a->path[tilepath]);
  path_b = &(cgf_b->path[tilepath]);

  u32 = (0xffffffff >> (start_step%32));
  start_mask = (uint64_t)u32 << 32;

  u32 = (0xffffffff << (32-((start_step+n_step)%32)));
  end_mask = (uint64_t)u32 << 32;

  start_block = start_step / 32;
  end_block = (start_step + n_step) / 32;

  s = start_block;

  x32 = ((path_a->vec[s] & start_mask) >> 32);
  y32 = ((path_b->vec[s] & start_mask) >> 32);
  k = NumberOfSetBits(x32 & y32);
  canonical_count += (32-(start_step%32)) - k;

  for (s=start_block+1; s<end_block; s++) {
    x32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
    y32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);
    k = NumberOfSetBits(x32 | y32);
    canonical_count += 32 - k;
  }

  if (s==end_block) {
    x32 = ((path_a->vec[s] & end_mask) >> 32);
    y32 = ((path_b->vec[s] & end_mask) >> 32);
    k = NumberOfSetBits(x32 | y32);
    canonical_count += ((start_step+n_step)%32) - k;
  }


  *n_match = canonical_count;

  return 0;
}

// Only consider either canonical tiles or
// cached overflows.  All otherws (final overflows, low quality
// tiles, etc.) will be ignored.
//
int cgf_tile_concordance_1(int *n_match, int *n_ovf,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath, int start_step, int n_step) {

  int i, j, k, bit_idx;
  int start_block, end_block, s;
  cgf_path_t *path_a, *path_b;
  uint64_t mask, z;
  uint32_t u32, x32, y32, fullx32, fully32;
  uint32_t lx32, ly32;

  uint32_t xor32, and32;

  uint64_t start_mask, end_mask;

  uint8_t hexit_a_n, hexit_a[8], hexit_b_n, hexit_b[8];
  int a_count, b_count;

  int a_ovf_loq = 0, a_ovf_hiq=0, a_ovf_complex=0;
  int b_ovf_loq = 0, b_ovf_hiq=0, b_ovf_complex=0;

  int canon_match_count=0, cache_match_count=0, ovf_count=0;
  int loq_cache_count=0, cache_ovf_count=0;

  unsigned char flag;

  int s_mod, e_mod;
  int skip_beg=0, use_end=32;

  int local_debug=1;

  path_a = &(cgf_a->path[tilepath]);
  path_b = &(cgf_b->path[tilepath]);

  start_block = start_step / 32;
  end_block = (start_step + n_step) / 32;

  for (s=start_block; s<=end_block; s++) {

    mask = 0xffffffff00000000;
    skip_beg = 0;
    use_end = 32;

    if (s==start_block) {
      u32 = (0xffffffff >> (start_step%32));
      mask &= (uint64_t)u32 << 32;

      skip_beg = start_step % 32;
    }

    if (s==end_block) {

      u32 = (0xffffffff << (32-((start_step+n_step)%32)));
      mask &= (uint64_t)u32 << 32;

      use_end = (start_step + n_step) % 32;
    }

    x32 = ((path_a->vec[s] & mask ) >> 32);
    y32 = ((path_b->vec[s] & mask ) >> 32);
    k = NumberOfSetBits(x32 | y32);
    canon_match_count += (32-skip_beg-(32-use_end)) - k;

    //DEBUG
    if (local_debug) {

      fullx32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      fully32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      printf(">> s: %i, k: %i, x32: %08x (%08x), y32: %08x (%08x), skip_beg: %d, use_end: %d, mask: %016" PRIx64 "\n",
          s, k,
          (unsigned int)x32, (unsigned int)fullx32,
          (unsigned int)y32, (unsigned int)fully32,
          skip_beg, use_end, mask);
    }

    if (k>0) {

      // need full vector
      //
      x32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      y32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      hexit_a_n = NumberOfSetBits(x32);
      hexit_b_n = NumberOfSetBits(y32);

      lx32 = path_a->vec[s] & 0xffffffff;
      ly32 = path_b->vec[s] & 0xffffffff;

      //DEBUG
      if (local_debug) {
        printf("  lx32: %08x, ly32: %08x\n", (unsigned int)lx32, (unsigned int)ly32);
      }

      for (i=0; i<8; i++) {
        hexit_a[7-i] = (uint8_t)((lx32 & (0xf << (4*i)))>>(4*i));
        hexit_b[7-i] = (uint8_t)((ly32 & (0xf << (4*i)))>>(4*i));
      }

      a_count=0;
      b_count=0;
      and32 = x32 & y32;

      for (i=31; i>=0; i--) {
        bit_idx = 31-i;

        //DEBUG
        if (local_debug) {
          printf("  [%i(%i)] (%c,%c:%c) a_count %i, b_count %i\n",
              i, bit_idx,
              //i,
              (x32&(1<<i)) ? '*' : '_',
              (y32&(1<<i)) ? '*' : '_',
              (and32&(1<<i)) ? '*' : '_', a_count, b_count);
          if (and32 & (1<<i)) {
            if (a_count<8) { printf("    a[%i]: %x\n", a_count, hexit_a[a_count]); }
            if (b_count<8) { printf("    b[%i]: %x\n", b_count, hexit_b[b_count]); }
          }
        }

        if (and32 & (1<<i)) {
          if ((a_count<8) && (b_count<8) &&
              (hexit_a[a_count] > 0) && (hexit_a[a_count] < 0xd) &&
              (hexit_b[b_count] > 0) && (hexit_b[b_count] < 0xd)) {

            if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
              cache_match_count += ((hexit_a[a_count] == hexit_b[b_count]) ? 1 : 0);

              //DEBUG
              if (local_debug) {
                printf("      cache_match_count++\n");
              }

            }
            else if (local_debug) {
              printf("      skipped (cache_match_count++)\n");
            }


          }
          else {
            flag = 0;

            if (a_count<8) {
              if      (hexit_a[a_count] == 0xe) { a_ovf_loq++; flag |= (1<<0); }
              else if (hexit_a[a_count] == 0xf) { a_ovf_hiq++; flag |= (1<<1); }
              else if (hexit_a[a_count] == 0xd) { a_ovf_complex++; flag |= (1<<2); }
            }

            if (b_count<8) {
              if      (hexit_b[b_count] == 0xe) { b_ovf_loq++; flag |= (1<<3); }
              else if (hexit_b[b_count] == 0xf) { b_ovf_hiq++; flag |= (1<<4); }
              else if (hexit_b[b_count] == 0xd) { b_ovf_complex++; flag |= (1<<5); }
            }

            if ((a_count<8) && (b_count<8)) {
              if (flag & ((1<<0) | (1<<3))) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  loq_cache_count++;

                  //DEBUG
                  if (local_debug) {
                    printf("      loq_cache_count++\n");
                  }

                }
                else if (local_debug) {
                  printf("      skipped (loq_cache_count++)\n");
                }

              }
              else if (flag & ((1<<1) | (1<<4))) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  ovf_count++;

                  //DEBUG
                  if (local_debug) {
                    printf("      ovf_count++\n");
                  }

                }
                else if (local_debug) {
                    printf("      skipped (ovf_count++)\n");
                }

              }
            }
            else {

              if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                cache_ovf_count++;

                if (local_debug) {
                  printf("        cache_ovf_count++\n");
                }

              }
              else if (local_debug) {
                printf("      skipped (cache_ovf_count++)\n");
              }

            }



          }

        }

        if (x32 & (1<<i)) { a_count++; }
        if (y32 & (1<<i)) { b_count++; }

      }

    }

  }

  *n_match = canon_match_count + cache_match_count;
  *n_ovf = ovf_count;

  return 0;
}

int cgf_final_overflow_scan_to_start(cgf_final_overflow_t *fin_ovf, int start_step) {
  int i, j, k, b;
  uint64_t tot_sz, data_sz;
  uint8_t *code, *data;

  tot_sz = fin_ovf->data_record_byte_len;
  data_sz = tot_sz - fin_ovf->data_record_n;

  code = fin_ovf->data_record->code;
  data = fin_ovf->data_record->data;

  //for (b=0; b<data_sz; ) { }

  return 0;
}

int cgf_cache_map_val(uint64_t vec_val, int ofst) {
  int i, count;
  unsigned char hx;
  uint64_t mask, x;

  //printf("??? vec_val %016llx, ofst %i\n", (unsigned long long int)vec_val, ofst);

  // canonical tile
  if ((vec_val & (((uint64_t)1)<<(32+ofst)))==0) {

    /*
    x = 1;
    x = ((uint64_t)1)<<(32+ofst);
    printf("????? %llu, %llu, %llu\n",
        (unsigned long long int)x,
        (unsigned long long int)vec_val & x,
        (unsigned long long int)(vec_val & (((uint64_t)1)<<(32+ofst))));

    printf("?????? canon??\n");
    */

    return 0;
  }

  for (i=0, count=0; i<ofst; i++) {
    if (vec_val & (1<<(32+i))) { count++; }
  }

  if (count>=8) { return -1; }

  //printf("?? count: %d\n", count);

  hx = (unsigned char)((vec_val & (0xf<<(count*4))) >> (count*4));
  return (int)hx;
}

int cgf_relative_overflow_count(uint64_t *vec, int step_start, int step_end) {
  int vec_idx, step_off;
  int cur_step, ovf_count=0;
  int cache_map_val;
  uint64_t vec_val;

  for (cur_step=step_start; cur_step<=step_end; cur_step++) {
    vec_idx = cur_step/32;
    step_off = cur_step%32;

    vec_val = vec[vec_idx];

    cache_map_val = cgf_cache_map_val(vec[vec_idx], step_off);
    if (cache_map_val==0) { continue; }
    if ((cache_map_val>0) && (cache_map_val<0xd)) { continue; }

    // complex tiles not implemented, ignore
    //
    if (cache_map_val==0xd) { continue; }

    ovf_count++;

  }

  return ovf_count;
}

//int cgf_overflow_variant_id(cgf_t *cgf, int tilepath, int step) {
int cgf_map_variant_id(cgf_t *cgf, int tilepath, int step) {
  int i, j, k, dn;
  uint64_t nblock, stride, byte_tot;
  uint32_t u32;
  int byte_offset=0;
  int map_skip_count;

  //printf("cps>> %x %x\n", tilepath, step);

  cgf_path_t *path;
  cgf_overflow_t *ovf;

  path = &(cgf->path[tilepath]);

  k = cgf_cache_map_val(path->vec[step/32], step%32);
  if ((k>=0) && (k<0xd)) {

    //DEBUG
    //printf("cp0>> %d (%x)\n", k, k);

    return k;
  }

  //printf("k %d\n", k);

  // complex tiles not supported
  //
  if (k==0xd) { return -2; }

  ovf = path->overflow;
  nblock = ovf->length;
  stride = ovf->stride;

  byte_tot = ovf->map_byte_count;

  for (k=0; k<nblock; k++) {
    if (step < ovf->position[k]) { break; }
  }
  k--;

  /*
  printf("idx %d, offset[%d]: %llu, position[%d]: %llu\n", k,
      k, (unsigned long long int)ovf->offset[k],
      k, (unsigned long long int)ovf->position[k]);
      */

  byte_offset = ovf->offset[k];

  map_skip_count = cgf_relative_overflow_count(path->vec, ovf->position[k], step);

  //printf("map_skip_count: %d, byte_offset: %x\n", map_skip_count, (int)byte_offset);

  k = 0;
  while ((k < map_skip_count) && (byte_offset < byte_tot)) {
    dn = dlug_convert_uint32(ovf->map + byte_offset, &u32);
    if (dn<=0) { return -1; }
    byte_offset += dn;

    //printf("  [%d] %x\n", k, u32);
    k++;
  }

  /*
  dn = dlug_convert_uint32(ovf->map + byte_offset, &u32);
  if (dn<=0) { return -1; }
  byte_offset += dn;
  */

  //printf("?? %x\n", u32);

  return (int)u32;

}

int cgf_final_overflow_match(cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int tilestep ) {

  return 0;
}

// ovf_step has [ step , codea, code b ]
// where codeX is -1 for overflow, -2 for complex and has the code otherwise.
//
int cgf_overflow_concordance(int *n_match, int *n_fin_ovf,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath,
    std::vector<int> &ovf_step) {
  int i, j, k, idx;
  int var_a, var_b, step;
  std::vector<int> fin_ovf_step;
  int match_count=0, fin_ovf_count=0;

  for (idx=0; idx<ovf_step.size(); idx+=3) {
    step = ovf_step[idx];
    var_a = ovf_step[idx+1];
    var_b = ovf_step[idx+2];

    // complex, ignore
    //
    if ((var_a<-1) || (var_b<-1)) { continue; }

    if (var_a<0) {
      var_a = cgf_map_variant_id(cgf_a, tilepath, step);
    }

    if (var_b<0) {
      var_b = cgf_map_variant_id(cgf_b, tilepath, step);
    }

    if ((var_a < 1024) && (var_b < 1024)) {
      if (var_a==var_b) {

        //DEBUG
        printf("%04x.%04x, ovf_conf++\n", tilepath, step);

        match_count++;
      }
    } else if ((var_a>=1024) && (var_b>=1024)) {
      fin_ovf_step.push_back(step);
      fin_ovf_count++;

      //DEBUG
      printf("%04x.%04x: fin_ovf queue\n", tilepath, step);
    }

  }

  for (i=0; i<fin_ovf_step.size(); i++) {
    if (cgf_final_overflow_match(cgf_a, cgf_b, tilepath, fin_ovf_step[i])) {
      match_count++;
    }
  }

  *n_match = match_count;

  return 0;
}

uint8_t cgf_loq_tile(cgf_t *cgf, int tilepath, int tilestep) {
  //uint8_t u8, z8;
  //u8 = cgf->path[tilepath].loq_info->loq_flag[tilestep/8];
  //z8 = (1<<(tilestep%8));
  return cgf->path[tilepath].loq_info->loq_flag[tilestep/8] & (1<<(tilestep%8));
}

// Only consider either canonical tiles,
// cached overflows or tile mapped overflows.
// All otherws (final overflows, low quality
// tiles, etc.) will be ignored.
//
int cgf_tile_concordance_2(int *n_match, int *n_ovf,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath, int start_step, int n_step) {

  int i, j, k, bit_idx;
  int start_block, end_block, s;
  cgf_path_t *path_a, *path_b;
  uint64_t mask, z;
  uint32_t u32, x32, y32, fullx32, fully32;
  uint32_t lx32, ly32;

  uint32_t xor32, and32;

  uint64_t start_mask, end_mask;

  uint8_t hexit_a_n, hexit_a[8], hexit_b_n, hexit_b[8];
  int a_count, b_count;

  int a_ovf_loq = 0, a_ovf_hiq=0, a_ovf_complex=0;
  int b_ovf_loq = 0, b_ovf_hiq=0, b_ovf_complex=0;

  int canon_match_count=0, cache_match_count=0, ovf_count=0;
  int loq_cache_count=0, cache_ovf_count=0;

  unsigned char flag;

  int s_mod, e_mod;
  int skip_beg=0, use_end=32;

  int local_debug=1;

  uint8_t *loq_flag_a, *loq_flag_b;

  std::vector<int> ovf_info;

  path_a = &(cgf_a->path[tilepath]);
  path_b = &(cgf_b->path[tilepath]);

  start_block = start_step / 32;
  end_block = (start_step + n_step) / 32;

  //loq_flag_a = cgf_a->loq_info->loq_flag;
  //loq_flag_b = cgf_b->loq_info->loq_flag;

  for (s=start_block; s<=end_block; s++) {

    mask = 0xffffffff00000000;
    skip_beg = 0;
    use_end = 32;

    if (s==start_block) {
      u32 = (0xffffffff >> (start_step%32));
      mask &= (uint64_t)u32 << 32;

      skip_beg = start_step % 32;
    }

    if (s==end_block) {

      u32 = (0xffffffff << (32-((start_step+n_step)%32)));
      mask &= (uint64_t)u32 << 32;

      use_end = (start_step + n_step) % 32;
    }

    x32 = ((path_a->vec[s] & mask ) >> 32);
    y32 = ((path_b->vec[s] & mask ) >> 32);
    k = NumberOfSetBits(x32 | y32);
    canon_match_count += (32-skip_beg-(32-use_end)) - k;

    //DEBUG
    if (local_debug) {
      fullx32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      fully32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      printf(">> s: %i, k: %i, x32: %08x (%08x), y32: %08x (%08x), skip_beg: %d, use_end: %d, mask: %016" PRIx64 "\n",
          s, k,
          (unsigned int)x32, (unsigned int)fullx32,
          (unsigned int)y32, (unsigned int)fully32,
          skip_beg, use_end, mask);
    }

    if (k>0) {

      // need full vector
      //
      x32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      y32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      hexit_a_n = NumberOfSetBits(x32);
      hexit_b_n = NumberOfSetBits(y32);

      lx32 = path_a->vec[s] & 0xffffffff;
      ly32 = path_b->vec[s] & 0xffffffff;

      //DEBUG
      if (local_debug) { printf("  lx32: %08x, ly32: %08x\n", (unsigned int)lx32, (unsigned int)ly32); }

      for (i=0; i<8; i++) {
        hexit_a[7-i] = (uint8_t)((lx32 & (0xf << (4*i)))>>(4*i));
        hexit_b[7-i] = (uint8_t)((ly32 & (0xf << (4*i)))>>(4*i));
      }

      a_count=0;
      b_count=0;
      and32 = x32 & y32;

      for (i=31; i>=0; i--) {
        bit_idx = 31-i;

        //DEBUG
        if (local_debug) {
          printf("  [%i(%i)] (%c,%c:%c) a_count %i, b_count %i\n",
              i, bit_idx,
              //i,
              (x32&(1<<i)) ? '*' : '_',
              (y32&(1<<i)) ? '*' : '_',
              (and32&(1<<i)) ? '*' : '_', a_count, b_count);
          if (and32 & (1<<i)) {
            if (a_count<8) { printf("    a[%i]: %x\n", a_count, hexit_a[a_count]); }
            if (b_count<8) { printf("    b[%i]: %x\n", b_count, hexit_b[b_count]); }
          }
        }

        if (and32 & (1<<i)) {
          if ((a_count<8) && (b_count<8) &&
              (hexit_a[a_count] > 0) && (hexit_a[a_count] < 0xd) &&
              (hexit_b[b_count] > 0) && (hexit_b[b_count] < 0xd)) {

            if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
              cache_match_count += ((hexit_a[a_count] == hexit_b[b_count]) ? 1 : 0);

              //DEBUG
              if (local_debug) { printf("      cache_match_count%s\n", (hexit_a[a_count] == hexit_b[b_count]) ? "++" : ".." ); }
            }
            else if (local_debug) { printf("      skipped (cache_match_count++)\n"); }

          }
          else {
            flag = 0;

            if (a_count<8) {
              if      (hexit_a[a_count] == 0xe) { a_ovf_loq++; flag |= (1<<0); }
              else if (hexit_a[a_count] == 0xf) { a_ovf_hiq++; flag |= (1<<1); }
              else if (hexit_a[a_count] == 0xd) { a_ovf_complex++; flag |= (1<<2); }
            }

            if (b_count<8) {
              if      (hexit_b[b_count] == 0xe) { b_ovf_loq++; flag |= (1<<3); }
              else if (hexit_b[b_count] == 0xf) { b_ovf_hiq++; flag |= (1<<4); }
              else if (hexit_b[b_count] == 0xd) { b_ovf_complex++; flag |= (1<<5); }
            }

            if ((a_count<8) && (b_count<8)) {

              // If either loq flags are set, we discard the pair for
              // our concordance count.
              //
              if (flag & ((1<<0) | (1<<3))) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  loq_cache_count++;

                  //DEBUG
                  if (local_debug) { printf("      loq_cache_count++\n"); }
                }
                else if (local_debug) { printf("      skipped (loq_cache_count++)\n"); }

              }

              // Both are high quiality overflow
              //
              else if ( ((flag & (1<<1))>>1) & ((flag & (1<<4))>>4) ) {

              //else if (flag & ((1<<1) | (1<<4))) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  ovf_count++;

                  // push step into vector for later processing
                  //
                  ovf_info.push_back(s*32 + bit_idx);
                  ovf_info.push_back(-1);
                  ovf_info.push_back(-1);

                  //DEBUG
                  if (local_debug) { printf("      ovf_count++ (hiq ovf)\n"); }
                }
                else if (local_debug) { printf("      skipped (ovf_count++)\n"); }

              }
            }
            else {

              if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                cache_ovf_count++;

                if ((!cgf_loq_tile(cgf_a, tilepath, s*32 + bit_idx)) &&
                    (!cgf_loq_tile(cgf_b, tilepath, s*32 + bit_idx))) {

                  // push step and information of variant types into vector for later processing
                  //
                  ovf_info.push_back(s*32 + bit_idx);
                  if (a_count<8) {
                    if ((hexit_a[a_count] > 0) && (hexit_a[a_count] < 0xd)) { ovf_info.push_back(hexit_a[a_count]); }
                    else if (hexit_a[a_count] == 0xf) { ovf_info.push_back(-1); }
                    else { ovf_info.push_back(-2); }
                  } else { ovf_info.push_back(-1); }

                  if (b_count<8) {
                    if ((hexit_b[b_count] > 0) && (hexit_b[b_count] < 0xd)) { ovf_info.push_back(hexit_b[b_count]); }
                    else if (hexit_b[b_count] == 0xf) { ovf_info.push_back(-1); }
                    else { ovf_info.push_back(-2); }
                  } else { ovf_info.push_back(-1); }

                  if (local_debug) { printf("        cache_ovf_count++\n"); }
                }
                else if (local_debug) { printf("      skipped (step %d %x) (cache_ovf_count++) (a)\n", s*32 + bit_idx, s*32 + bit_idx); }

              }
              else if (local_debug) { printf("      skipped (step %d %x) (cache_ovf_count++) (b)\n", s*32 + bit_idx, s*32 + bit_idx); }

            }

          }

        }

        if (x32 & (1<<i)) { a_count++; }
        if (y32 & (1<<i)) { b_count++; }

      }

    }

  }

  for (i=0; i<ovf_info.size(); i++) {
    printf("ovf_info[%i]: %x\n", i, ovf_info[i]);
  }

  cgf_overflow_concordance(&k, &j, cgf_a, cgf_b, tilepath, ovf_info);

  printf(">>>> k %d, j %d\n", k, j);


  *n_match = canon_match_count + cache_match_count;
  *n_ovf = ovf_count;

  return 0;
}

int main(int argc, char **argv) {
  int i, j, k;
  FILE *fp=stdin;
  char *input_fn = NULL;
  char ch;
  cgf_t *cgf, *cgf_a, *cgf_b;;
  int debug_print = 0, stats_print=0;

  cgf_t **cgfa;
  int n_cgfa=3;

  int n_match, n_ovf;

  int tilepath= -1, tilestep=-1;
  int tilevariant_flag = 0;

  while ((ch=getopt(argc, argv, "hvi:DSVp:s:")) != -1) switch (ch) {
    case 'h':
      show_help();
      exit(0);
      break;

    case 'p':
      tilepath = atoi(optarg);
      break;
    case 's':
      tilestep = atoi(optarg);
      break;

    case 'V':
      tilevariant_flag = 1;
      break;

    case 'D':
      debug_print = 1;
      break;
    case 'S':
      stats_print=1;
      break;
    case 'i':
      input_fn = strdup(optarg);
      break;
    case 'v':
      break;
    default:
      break;
  }


  if (input_fn!=NULL) {
    if (!(fp = fopen(input_fn, "r"))) {
      perror(input_fn);
      show_help();
      exit(1);
    }
  } else if (isatty(fileno(stdin))) {
    show_help();
    exit(1);
  }

  //---
  //
  cgf = load_cgf(fp);

  cgf_b = load_cgf_fn("data/hu826751-GS03052-DNA_B01.cgf");
  //cgf_b = load_cgf_fn("data/hu0211D6-GS01175-DNA_E02.cgf");

  /*
  //cgf_tile_concordance_0(&k, cgf, cgf_b, 0, 3, 5000);
  cgf_tile_concordance_0(&k, cgf, cgf_b, 1, 3, 10000);
  //cgf_tile_concordance_0(&k, cgf, cgf_b, 3, 3, 5000);
  printf(">>> %d\n", k);
  */

  // testing cgf_tile_concordance_0
  //
  /*
  j=0;
  for (i=0; i<cgf->path_count; i++) {
    cgf_tile_concordance_0(&k, cgf, cgf_b, i, 0, cgf->path[i].n_tile);
    printf(">>> %d\n", k);
    j+=k;
  }

  printf(">>>>> %i\n", j);
  */

  // testing cgf_tile_concordance_1
  //
  /*
  if (!debug_print) {
    cgf_tile_concordance_0(&k, cgf, cgf_b, 1, 5607, 104);

    cgf_tile_concordance_1(&n_match, &n_ovf,
        cgf, cgf_b,
        1, 5607, 104);
        //1, 5607, 5);
        //1, 5600, 100);
    printf("canon_match: %i, n_match: %i, n_ovf: %i\n", k, n_match, n_ovf);
  }
  */

  if (tilevariant_flag) {
    j = cgf_map_variant_id(cgf, tilepath, tilestep);
    printf(">>> %04x.%04x: %d (%x)\n", tilepath, tilestep, j, j);
    exit(0);
  }

  if (!debug_print) {
    cgf_tile_concordance_2(&n_match, &n_ovf,
        cgf, cgf_b,
        1, 5607, 104);
        //1, 5607, 5);
        //1, 5600, 100);
    printf("n_match: %i, n_ovf: %i\n", n_match, n_ovf);
  }

  /*
  j=0;
  for (i=0; i<cgf->path_count; i++) {
    cgf_tile_concordance_1(&n_match, &n_ovf,
        cgf, cgf_b,
        i, 0, cgf->path[i].n_tile);
    printf(">>> %d\n", k);
    j+=k;
  }

  printf(">>>>> %i\n", j);
  */



  /*
  cgfa = (cgf_t **)malloc(sizeof(cgf_t *)*n_cgfa);
  for (i=0; i<n_cgfa; i++) {
    cgfa[i] = load_cgf_fn(input_fn);
    if (cgfa[i]==NULL) { printf("nope\n"); }
  }
  printf("ok\n");
  */

  if (debug_print) { debug_print_cgf(cgf); }
  if (stats_print) { stats_print_cgf(cgf); }

  if (fp!=stdin) { fclose(fp); }

}
