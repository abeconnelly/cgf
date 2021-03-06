#ifndef CGT_INCLUDE
#define CGT_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include <vector>

#include "dlug.h"


typedef struct cgf_string_type  {
  int n;
  unsigned char *s;
} cgf_string_t;

typedef struct cgf_overflow_type {
  uint64_t length;
  uint64_t stride;
  uint64_t map_byte_count;
  uint64_t *offset;
  uint64_t *position;
  uint8_t *map;
} cgf_overflow_t;

//----------

typedef struct cgf_data_record_type {
  uint8_t *code;
  uint8_t *data;
} cgf_data_record_t;

typedef struct cgf_final_overflow_type {
  uint64_t data_record_n;
  uint64_t data_record_byte_len;
  cgf_data_record_t *data_record;
} cgf_final_overflow_t;

//----------

typedef struct cgf_final_overflow_map_opt_type {
  uint64_t length;
  uint64_t stride;
  uint64_t *offset;
  uint64_t *tile_position;
  cgf_data_record_t *data_record;
} cgf_final_overflow_map_opt_t;

//---------

typedef struct cgf_low_quality_info_type {
  uint64_t count;
  uint64_t code;
  uint64_t stride;
  uint64_t *offset;
  uint64_t *step_position;
  uint8_t *hom_flag;

  uint64_t loq_flag_byte_count;
  uint8_t *loq_flag;

  uint64_t loq_info_byte_count;
  uint8_t *loq_info;

} cgf_low_quality_info_t;


//---------


typedef struct cgf_path_type {
  cgf_string_t name;
  uint64_t n_tile;
  uint64_t *vec;

  cgf_overflow_t                *overflow;
  cgf_final_overflow_t          *final_overflow;
  cgf_final_overflow_map_opt_t  *final_overflow_opt;
  cgf_low_quality_info_t        *loq_info;

} cgf_path_t;


typedef struct cgf_type {
  uint64_t magic;
  cgf_string_t cgf_version;
  cgf_string_t lib_version;
  uint64_t path_count;

  uint64_t tile_map_len;
  unsigned char *tile_map_bytes;
  int ***tile_map;
  int n_tile_map;

  uint64_t *step_per_path;
  uint64_t *path_struct_offset;
  cgf_path_t *path;

} cgf_t;

int cgf_unpack_tile_map(cgf_t *cgf);


int cgf_read_dlug(FILE *fp, unsigned char *buf);
int cgf_read_string(FILE *fp, cgf_string_t *s);
int cgf_read_uint64(FILE *fp, uint64_t *u);
int cgf_load_overflow(FILE *fp, cgf_overflow_t *ovf);
int cgf_load_final_overflow(FILE *fp, cgf_final_overflow_t *fin_ovf);
int cgf_load_low_quality_info(FILE *fp, cgf_low_quality_info_t *loq_info);
cgf_t *load_cgf(FILE *fp);
cgf_t *load_cgf_buf(FILE *fp);
cgf_t *load_cgf_fn(const char *fn);

int cgf_tile_band(cgf_t *cgf, int tilepath, int tilestep_beg, int tilestep_n, std::vector<int> *allele);
int cgf_loq_tile_band(cgf_t *cgf, int tilepath, int tilestep_beg, int tilestep_n, std::vector<int> *allele, std::vector< std::vector<int> > *loq_allele);


int cgf_print_tile_map(cgf_t *cgf);
void cgf_print_overflow(cgf_overflow_t *ovf, int tilepath);
void cgf_print_final_overflow(cgf_final_overflow_t *fin_ovf, int tilepath);
int cgf_print_low_quality_info(cgf_low_quality_info_t *loq_info, int tilepath);
void debug_print_cgf(cgf_t *cgf);
void stats_print_cgf(cgf_t *cgf);


int cgf_json_info_print(cgf_t *cgf);


//void show_help(void);


int cgf_unpack_tile_map(cgf_t *cgf);
int cgf_tile_concordance_0(int *n_match, cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int start_step, int n_step);


int cgf_tile_concordance_1(int *n_match, int *n_ovf, cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int start_step, int n_step);


int cgf_final_overflow_scan_to_start(cgf_final_overflow_t *fin_ovf, int start_step);
int cgf_cache_map_val(uint64_t vec_val, int ofst);
int cgf_relative_overflow_count(uint64_t *vec, int step_start, int step_end);
int is_canonical_tile(uint64_t vec_val, int ofst);
int cgf_map_variant_ids(cgf_t *cgf, int tilepath, std::vector<int> &step_vec, std::vector<int> &step_varid);
int cgf_map_variant_id(cgf_t *cgf, int tilepath, int step);
int cgf_final_overflow_map0_peel(uint8_t *bytes, int *anchor_step, int *n_allele, std::vector<int> *allele);
int cgf_final_overflow_match(cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int tilestep);
int cgf_overflow_concordance_2(int *n_match, cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, std::vector<int> &ovf_step);
int cgf_overflow_concordance(int *n_match, cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, std::vector<int> &ovf_step);
int cgf_tile_concordance_2(int *n_match, int *n_loq, cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int start_step, int n_step);
int cgf_final_overflow_step_offset(cgf_t *cgf, int tilepath, int tilestep);
void test_lvl2(cgf_t *cgf, cgf_t *cgf_b);

uint8_t cgf_loq_tile(cgf_t *cgf, int tilepath, int tilestep);
int cgf_expand_loq_info(cgf_t *cgf, int tilepath, int tilestep, std::vector< std::vector<int> > *v);
int cgf_loq_offset(cgf_t *cgf, int tilepath, int tilestep);
int cgf_loq_offset_2(cgf_t *cgf, int tilepath, int tilestep);

int cgf_loq_count(cgf_t *cgf, int tilepath, int tilestep, int n_tilestep);

#endif


