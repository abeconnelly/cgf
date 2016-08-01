#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include "cgb.hpp"
#include "dlug.h"

/*
void show_help() {
  printf("\n");
  printf("compact genome tool\n");
  printf("\n");
  printf("usage:\n");
  printf("  -i cgf        input cgf file\n");
  printf("  [-l lvl]      concordance level (0,1,2)\n");
  printf("  [-p tilepath] tile path\n");
  printf("  [-s tilestep] tile step\n");
  printf("  [-n n_step]   n tile steps\n");
  printf("  [-B]          band flag\n");
  printf("  [-C]          single tile path concordance\n");
  printf("  [-D]          debug print\n");
  printf("  [-S]          stats print\n");
  printf("  [-L]          low quality flag\n");
  printf("  [-k]          knot flag\n");
  printf("  [-j]          print JSON info\n");
  printf("  [-h]          help\n");
  printf("\n");
}
*/

int cgf_json_info_print(cgf_t *cgf) {
  int i, n;

  printf("{\n");
  printf("  \"CGFVersion\":\"%s\",\n", cgf->cgf_version.s);
  printf("  \"CGFLibraryVersion\":\"%s\",\n", cgf->lib_version.s);
  printf("  \"PathCount\":%" PRId64 ",\n", cgf->path_count);
  printf("  \"StepPerPath\":[");

  for (i=0; i<cgf->path_count; i++) {
    if (i>0) { printf(","); }

    if ((i%16)==0) {
      printf("\n");
      printf("    ");
    } else {
      printf(" ");
    }

    printf("%" PRId64, cgf->step_per_path[i]);
  }
  printf("\n  ]\n");
  printf("}\n");
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

//-----------

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


