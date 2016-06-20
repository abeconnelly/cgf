#ifndef CGT_INCLUDE
#define CGT_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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


int cgf_print_tile_map(cgf_t *cgf);
void cgf_print_overflow(cgf_overflow_t *ovf, int tilepath);
void cgf_print_final_overflow(cgf_final_overflow_t *fin_ovf, int tilepath);
int cgf_print_low_quality_info(cgf_low_quality_info_t *loq_info, int tilepath);
void debug_print_cgf(cgf_t *cgf);
void stats_print_cgf(cgf_t *cgf);

void show_help(void);


#endif

