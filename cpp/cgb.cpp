#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include "cgb.hpp"
#include "dlug.h"

// Unpacks the tile map stored as bytes in
// `tile_map_bytes` into the `tile_map` structure.
//
// The `tile_map` holds the 'knot' information for each
// of the first N tile variants.  The lowest level arrays
// hold the length of the array as the first entry with
// the subsequent entries alternating between the variant
// and span.  For example:
//
// [idx_0]:
//   [
//     [ n_{idx_0,0}, var_{idx_0,0,0}, span_{idx_0,0,0}, var_{idx_0,0,1}, span_{idx_0,0,1}, ..., var_{idx_0,0,n-1}, span_{idx_0,0,n-1} ],
//     [ n_{idx_0,1}, var_{idx_0,1,0}, span_{idx_0,1,0}, var_{idx_0,1,1}, span_{idx_0,1,1}, ..., var_{idx_0,1,n-1}, span_{idx_0,1,n-1} ],
//   ],
// [idx_1]:
//   [
//     [ n_{idx_1,0}, var_{idx_1,0,0}, span_{idx_1,0,0}, var_{idx_1,0,1}, span_{idx_1,0,1}, ..., var_{idx_1,0,n-1}, span_{idx_1,0,n-1} ],
//     [ n_{idx_1,1}, var_{idx_1,1,0}, span_{idx_1,1,0}, var_{idx_1,1,1}, span_{idx_1,1,1}, ..., var_{idx_1,1,n-1}, span_{idx_1,1,n-1} ],
//   ],
//
// ...
//
// [idx_{N-1}]:
//   [
//     [ n_{idx_{N-1},0}, var_{idx_{N-1},0,0}, span_{idx_{N-1},0,0}, var_{idx_{N-1},0,1}, span_{idx_{N-1},0,1}, ..., var_{idx_{N-1},0,n-1}, span_{idx_{N-1},0,n-1} ],
//     [ n_{idx_{N-1},1}, var_{idx_{N-1},1,0}, span_{idx_{N-1},1,0}, var_{idx_{N-1},1,1}, span_{idx_{N-1},1,1}, ..., var_{idx_{N-1},1,n-1}, span_{idx_{N-1},1,n-1} ],
//   ]
//
//
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

//-----

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

inline int NumberOfSetBits(uint32_t u)
{
  u = u - ((u >> 1) & 0x55555555);
  u = (u & 0x33333333) + ((u >> 2) & 0x33333333);
  return (((u + (u >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}

/*
static const unsigned char BitsSetTable256[256] =
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

inline int NumberOfSetBits(uint32_t u) {

  // Option 1:
  return BitsSetTable256[u & 0xff] +
      BitsSetTable256[(u >> 8) & 0xff] +
      BitsSetTable256[(u >> 16) & 0xff] +
      BitsSetTable256[u >> 24];

  // Option 2:
  unsigned char * p = (unsigned char *) &u;
  return BitsSetTable256[p[0]] +
      BitsSetTable256[p[1]] +
      BitsSetTable256[p[2]] +
      BitsSetTable256[p[3]];
}
*/


// This is slower than the above but is more explicit
//
inline int NumberOfSetBits8(uint8_t u)
{
  u = (u & 0x55) + ((u>>1) & 0x55);
  u = (u & 0x33) + ((u>>2) & 0x33);
  u = (u & 0x0f) + ((u>>4) & 0x0f);
  return u;
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

  int i, j, k, bit_idx, t;
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

  uint32_t u32mask;

  //int local_debug = 0;

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

    /*
    if (local_debug) {

      fullx32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      fully32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      printf(">> s: %i, k: %i, x32: %08x (%08x), y32: %08x (%08x), skip_beg: %d, use_end: %d, mask: %016" PRIx64 "\n",
          s, k,
          (unsigned int)x32, (unsigned int)fullx32,
          (unsigned int)y32, (unsigned int)fully32,
          skip_beg, use_end, mask);
    }
    */

    if (k>0) {

      // need full vector
      //
      x32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      y32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      hexit_a_n = NumberOfSetBits(x32);
      hexit_b_n = NumberOfSetBits(y32);

      lx32 = path_a->vec[s] & 0xffffffff;
      ly32 = path_b->vec[s] & 0xffffffff;

      //if (local_debug) { printf("  lx32: %08x, ly32: %08x\n", (unsigned int)lx32, (unsigned int)ly32); }

      for (i=0; i<8; i++) {
        //hexit_a[7-i] = (uint8_t)((lx32 & (0xf << (4*i)))>>(4*i));
        //hexit_b[7-i] = (uint8_t)((ly32 & (0xf << (4*i)))>>(4*i));
        t = 4*i;
        hexit_a[7-i] = (uint8_t)((lx32 & (0xf << (t)))>>(t));
        hexit_b[7-i] = (uint8_t)((ly32 & (0xf << (t)))>>(t));
      }

      a_count=0;
      b_count=0;
      and32 = x32 & y32;

      for (i=31; i>=0; i--) {
        bit_idx = 31-i;

        u32mask = (((uint32_t)1)<<i);

        /*
        if (local_debug) {
          printf("  [%i(%i)] (%c,%c:%c) a_count %i, b_count %i\n",
              i, bit_idx,
              (x32&(1<<i)) ? '*' : '_',
              (y32&(1<<i)) ? '*' : '_',
              (and32&(1<<i)) ? '*' : '_', a_count, b_count);
          if (and32 & (1<<i)) {
            if (a_count<8) { printf("    a[%i]: %x\n", a_count, hexit_a[a_count]); }
            if (b_count<8) { printf("    b[%i]: %x\n", b_count, hexit_b[b_count]); }
          }
        }
        */

        //if (and32 & (1<<i)) {
        if (and32 & u32mask) {
          if ((a_count<8) && (b_count<8) &&
              (hexit_a[a_count] > 0) && (hexit_a[a_count] < 0xd) &&
              (hexit_b[b_count] > 0) && (hexit_b[b_count] < 0xd)) {

            if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
              cache_match_count += ((hexit_a[a_count] == hexit_b[b_count]) ? 1 : 0);

              //if (local_debug) { printf("      cache_match_count++\n"); }

            }
            //else if (local_debug) { printf("      skipped (cache_match_count++)\n"); }


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

                  //if (local_debug) { printf("      loq_cache_count++\n"); }

                }
                //else if (local_debug) { printf("      skipped (loq_cache_count++)\n"); }

              }
              else if (flag & ((1<<1) | (1<<4))) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  ovf_count++;

                  //if (local_debug) { printf("      ovf_count++\n"); }

                }
                //else if (local_debug) { printf("      skipped (ovf_count++)\n"); }

              }
            }
            else {

              if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                cache_ovf_count++;

                //if (local_debug) { printf("        cache_ovf_count++\n"); }

              }
              //else if (local_debug) { printf("      skipped (cache_ovf_count++)\n"); }

            }

          }

        }

        //if (x32 & (1<<i)) { a_count++; }
        //if (y32 & (1<<i)) { b_count++; }

        if (x32 & u32mask) { a_count++; }
        if (y32 & u32mask) { b_count++; }

      }

    }

  }

  *n_match = canon_match_count + cache_match_count;
  *n_ovf = ovf_count;

  return 0;
}

// Still in development
//
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

// Get the cached value in `vec_val`.
// Return:
//  0         : spanning tile
//  1-c       : cache knot id
//  d         : complex tile
//  e         : loq overflow
//  f         : hiq overflow
// -1         : canonical tile
// -2         : cache overflow
//
inline int cgf_cache_map_val(uint64_t vec_val, int ofst) {
  int i, count, shft;
  unsigned char hx;
  uint64_t mask, x;
  //int local_debug = 0;

  uint32_t u32, mask32;
  uint32_t tst;

  u32 = (uint32_t)(vec_val>>32);

  // canonical tile
  //
  //if ((vec_val & (((uint64_t)1)<<(32+ofst)))==0) { return -1; }
  if ((u32 & (1<<ofst))==0) { return -1; }

  //if (local_debug) { printf("    vec_val %016" PRIx64 ", ofst %i\n", vec_val, ofst); }

  mask32 = (((uint32_t)0xffffffff)>>(31-ofst));
  u32 &= mask32;

  count = NumberOfSetBits(u32);

  //if (local_debug) { printf("    count %i\n", count); }

  // overflow
  //
  if (count>8) { return -2; }

  // error, no cache map val at location
  //
  if (count<=0) { return -3; }

  shft=count-1;
  mask = 0xf;
  mask = mask<<(shft*4);
  mask = vec_val & mask;


  //if (local_debug) { printf("    masked %016" PRIx64 ", count %i\n", vec_val & mask, count); }

  mask = mask >> (shft*4);
  hx = (unsigned char)(mask & 0xf);

  return (int)hx;
}

// Count overflow entries from step_start to step_end, inclusive.
// This is needed for determining where in the OverflowMap (or FinalOverflowMap)
// the appropriate entry is.
//
int cgf_relative_overflow_count(uint64_t *vec, int step_start, int step_end) {
  int vec_idx, step_off;
  int cur_step, ovf_count=0;
  int cache_map_val;
  uint64_t vec_val;
  uint32_t canon_bits, ovf_bits;

  //int local_debug = 0;

  for (cur_step=step_start; cur_step<=step_end; cur_step++) {
    //vec_idx = cur_step/32;
    //step_off = cur_step%32;

    vec_idx = cur_step >> 5;
    step_off = cur_step & 0x1f;

    vec_val = vec[vec_idx];

    /*
    if (local_debug) {
      canon_bits = (uint32_t)(vec_val>>32);
      ovf_bits = (uint32_t)(vec_val&0xffffffff);

      printf("  [%i] %08x %08x\n", cur_step, (unsigned int)canon_bits, (unsigned int)ovf_bits);
    }
    */



    //cache_map_val = cgf_cache_map_val(vec[vec_idx], step_off);
    cache_map_val = cgf_cache_map_val(vec_val, step_off);

    /*
    if (local_debug) {
      printf("  cgf_relative_overflow_count step %x {%x,%x} cache_map_val %x\n", cur_step, vec_idx, step_off, cache_map_val);
    }
    */

    // canonical tile (no cache map entry), skip
    //
    if (cache_map_val==-1) { continue; }

    // spanning tile
    //
    if (cache_map_val==0) { continue; }

    // valid cache entry
    //
    if ((cache_map_val>0) && (cache_map_val<0xd)) { continue; }

    // complex tiles not implemented, ignore
    //
    if (cache_map_val==0xd) { continue; }


    // cache map val -2 is overflow of cache,
    // 0xf is hiq overflow, 0xe is loq overflow.
    //
    if ((cache_map_val==0xf) || (cache_map_val==0xe) || (cache_map_val==-2)) {
      ovf_count++;

      //if (local_debug) { printf("  > ovf_count++ (--> %d)\n", ovf_count); }

    }

  }

  /*
  if (local_debug) {
    printf("  >> cgf_relative_overflow_count step %x {%x,%x} cache_map_val %x, ovf_count %d\n", cur_step, vec_idx, step_off, cache_map_val, ovf_count);
  }
  */


  return ovf_count;
}

// Helper function.  Returns non-zero tile offset
// represents a canonical tile.
//
int is_canonical_tile(uint64_t vec_val, int ofst) {
  return (vec_val) & ( ((uint64_t)1)<<(32+ofst) );
}

// Find variant id of tilepath.tilestep in structure.
// First determine whether it's a canonical tile or
// resides in the cache and if it is, return the value.
// Otherwise, start looking in the overflow and final
// overflow structures.
//
int cgf_map_variant_ids(cgf_t *cgf, int tilepath, std::vector<int> &step_vec, std::vector<int> &step_varid) {
  int i, j, k, dn;
  uint64_t nblock, stride, byte_tot;
  uint32_t u32;
  int byte_offset=0;
  int map_skip_count;

  //int local_debug = 0;
  int step, prev_ovf_step;

  int actual_ovf_count=0;
  int step_idx;

  cgf_path_t *path;
  cgf_overflow_t *ovf;

  path = &(cgf->path[tilepath]);

  for (step_idx=0; step_idx<step_vec.size(); step_idx++) {

    step = step_vec[step_idx];

    if (is_canonical_tile(path->vec[step/32], (step%32))) {
      step_varid.push_back(0);

      //if (local_debug) { printf("  canon: step %x\n", step); }

      continue;
    }

    k = cgf_cache_map_val(path->vec[step/32], step%32);

    // canonical tile
    //
    if (k==-1) {
      step_varid.push_back(0);
      continue;
    }

    if ((k>=0) && (k<0xd)) {

      /*
      if (local_debug) {
        printf("  cgf_map_variant_id %x.%x got cache %x\n", tilepath, step, k);
      }
      */

      step_varid.push_back( (k==0) ? -1 : k );
      continue;
    }


    // complex tiles not supported
    //
    if (k==0xd) {
      step_varid.push_back(-2);
      continue;
    }

    if (actual_ovf_count==0) {
      ovf = path->overflow;
      nblock = (ovf->length + ovf->stride - 1) / ovf->stride;
      stride = ovf->stride;

      byte_tot = ovf->map_byte_count;

      for (k=0; k<nblock; k++) {
        if (step < ovf->position[k]) { break; }
      }
      k--;

      /*
      if (local_debug) {
        printf("k block %i (step %d (%x), position[%d] %d (%x))\n", k, step, step, k, (int)ovf->position[k], (int)ovf->position[k]);
      }
      */

      byte_offset = ovf->offset[k];

      prev_ovf_step = ovf->position[k];
    }

    /*
    if (local_debug) {
      printf("byte offset %d (%x)\n", (int)byte_offset, (int)byte_offset);
    }
    */

    map_skip_count = cgf_relative_overflow_count(path->vec, prev_ovf_step, step);

    /*
    if (local_debug) {
      printf("  cgf_map_variant_id %x.%x map_skip_count %d\n", tilepath, step, map_skip_count);
    }
    */

    k = 0;
    while ((k < map_skip_count) && (byte_offset < byte_tot)) {
      dn = dlug_convert_uint32(ovf->map + byte_offset, &u32);
      if (dn<=0) { return -1; }

      //if (local_debug) { printf("  map[%d(%x)] %i, k:%d\n", (int)byte_offset, (int)byte_offset, (int)u32, k); }

      byte_offset += dn;

      k++;
    }

    /*
    if (local_debug) {
      printf("  cgf_map_variant_id %x.%x mapval %i (skipped %d)\n", tilepath, step, (int)u32, k);
    }
    */

    actual_ovf_count++;

    prev_ovf_step = step+1;
    step_varid.push_back((int)u32);
  }

  return actual_ovf_count;

}

// Find variant id of tilepath.tilestep in structure.
// First determine whether it's a canonical tile or
// resides in the cache and if it is, return the value.
// Otherwise, start looking in the overflow and final
// overflow structures.
//
// x
//
int cgf_map_variant_id(cgf_t *cgf, int tilepath, int step) {
  int i, j, k, dn;
  uint64_t nblock, stride, byte_tot;
  uint32_t u32;
  int byte_offset=0;
  int map_skip_count;

  //int local_debug = 0;

  cgf_path_t *path;
  cgf_overflow_t *ovf;

  path = &(cgf->path[tilepath]);

  if (is_canonical_tile(path->vec[step/32], (step%32))) { return 0; }

  k = cgf_cache_map_val(path->vec[step/32], step%32);

  // canonical tile
  //
  if (k==-1) { return 0; }

  if ((k>=0) && (k<0xd)) {

    /*
    if (local_debug) {
      printf("  cgf_map_variant_id %x.%x got cache %x\n", tilepath, step, k);
    }
    */

    // trailing spanning tile
    //
    if (k==0) { return -1; }

    return k;
  }


  // complex tiles not supported
  //
  if (k==0xd) { return -2; }

  ovf = path->overflow;
  nblock = (ovf->length + ovf->stride - 1) / ovf->stride;
  stride = ovf->stride;

  byte_tot = ovf->map_byte_count;

  for (k=0; k<nblock; k++) {
    if (step < ovf->position[k]) { break; }
  }
  k--;

  /*
  if (local_debug) {
    printf("k block %i (step %d (%x), position[%d] %d (%x))\n", k, step, step, k, (int)ovf->position[k], (int)ovf->position[k]);
  }
  */

  byte_offset = ovf->offset[k];

  /*
  if (local_debug) {
    printf("byte offset %d (%x)\n", (int)byte_offset, (int)byte_offset);
  }
  */

  map_skip_count = cgf_relative_overflow_count(path->vec, ovf->position[k], step);

  /*
  if (local_debug) {
    printf("  cgf_map_variant_id %x.%x map_skip_count %d\n", tilepath, step, map_skip_count);
  }
  */

  k = 0;
  while ((k < map_skip_count) && (byte_offset < byte_tot)) {
    dn = dlug_convert_uint32(ovf->map + byte_offset, &u32);
    if (dn<=0) { return -1; }

    //if (local_debug) { printf("  map[%d(%x)] %i, k:%d\n", (int)byte_offset, (int)byte_offset, (int)u32, k); }

    byte_offset += dn;

    k++;
  }

  /*
  if (local_debug) {
    printf("  cgf_map_variant_id %x.%x mapval %i (skipped %d)\n", tilepath, step, (int)u32, k);
  }
  */

  return (int)u32;

}

int cgf_final_overflow_map0_peel(uint8_t *bytes,
    int *anchor_step, int *n_allele,
    std::vector<int> *allele) {
  int i, j, k;
  int dn, n=0;
  int vid, span;
  uint32_t u32, len, aa;

  //int local_debug = 0;

  /*
  if (local_debug) {
    if (allele!=NULL) {
      printf(">>>> %p\n", allele);
      printf("%i\n", (int)allele[0].size());
      printf("%i\n", (int)allele[1].size());

      if (allele[0].size()>0) {
        printf(">>>> 0: %d\n", allele[0][0]);
      }

      if (allele[1].size()>0) {
        printf(">>>> 1: %d\n", allele[1][0]);
      }
    }
  }
  */

  dn = dlug_convert_uint32(bytes + n, &u32);
  if (dn<=0) { return -1; }
  n += dn;

  *anchor_step = (int)u32;

  /*
  if (local_debug) {
    printf("  anchor_step %x\n", (int)u32);
  }
  */

  dn = dlug_convert_uint32(bytes + n, &u32);
  if (dn<=0) { return -1; }
  n += dn;

  *n_allele = (int)u32;
  aa = u32;

  /*
  if (local_debug) {
    printf("  n_allele %i\n", (int)aa);
  }
  */

  for (i=0; i<aa; i++) {
    dn = dlug_convert_uint32(bytes + n, &u32);
    if (dn<=0) { return -1; }
    n += dn;

    len=u32;

    for (j=0; j<len; j++) {
      dn = dlug_convert_uint32(bytes + n, &u32);
      if (dn<=0) { return -1; }
      n += dn;

      vid = (int)u32;

      dn = dlug_convert_uint32(bytes + n, &u32);
      if (dn<=0) { return -1; }
      n += dn;

      span = (int)u32;

      if (allele!=NULL) {
        allele[i].push_back(vid);
        allele[i].push_back(span);
      }

    }
  }

  return n;
}

int cgf_final_overflow_knot(cgf_t *cgf, int tilepath, int tilestep, std::vector<int> *knot) {
  int i, j, k;
  uint64_t n, byte_len;
  uint8_t *code;
  uint8_t *map;
  int rec;
  int step;
  int dn;
  int byte_offset;
  cgf_final_overflow_t *fin_ovf;

  //int local_debug = 0;

  knot[0].clear();
  knot[1].clear();

  fin_ovf = cgf->path[tilepath].final_overflow;

  n = fin_ovf->data_record_n;
  byte_len = fin_ovf->data_record_byte_len;

  code = fin_ovf->data_record->code;
  map  = fin_ovf->data_record->data;

  byte_offset = 0;

  /*
  if (local_debug) {
    printf(">>> cgf_fin_ovf_knot %04x.%04x\n", tilepath, tilestep);
  }
  */

  rec = 0;
  step = -1;
  while ((byte_offset < byte_len) && (step < tilestep) && (rec < n)) {

    knot[0].clear();
    knot[1].clear();

    if (code[rec]==0) {
      dn = cgf_final_overflow_map0_peel(map + byte_offset, &step, &k, knot);
      if (dn<=0) { return 0; }
      byte_offset += dn;

      if (k!=2) { return 0; }
    } else { return 0; }

    rec++;
  }

  /*
  if (local_debug) {
    printf("fin: %04x.%04x: fin ovf: rec %i, (step %x)\n", tilepath, tilestep, rec, step );
  }
  */

  return 1;

}

// Determine if the tilepath.tilestep for cgf_a and cgf_b match.
//
int cgf_final_overflow_match(cgf_t *cgf_a, cgf_t *cgf_b, int tilepath, int tilestep) {
  int i, j, k;
  uint64_t n_a, n_b, byte_len_a, byte_len_b;
  uint8_t *code_a, *code_b;
  uint8_t *map_a, *map_b;
  int rec_a, rec_b;
  int step_a, step_b;
  int dn;
  int byte_offset_a, byte_offset_b;
  cgf_final_overflow_t *fin_ovf_a, *fin_ovf_b;

  std::vector<int> knot_a[2], knot_b[2];

  //int local_debug = 0;

  fin_ovf_a = cgf_a->path[tilepath].final_overflow;
  fin_ovf_b = cgf_b->path[tilepath].final_overflow;

  n_a = fin_ovf_a->data_record_n;
  byte_len_a = fin_ovf_a->data_record_byte_len;

  n_b = fin_ovf_b->data_record_n;
  byte_len_b = fin_ovf_b->data_record_byte_len;

  code_a = fin_ovf_a->data_record->code;
  map_a  = fin_ovf_a->data_record->data;

  code_b = fin_ovf_b->data_record->code;
  map_b  = fin_ovf_b->data_record->data;

  byte_offset_a = 0;
  byte_offset_b = 0;

  /*
  if (local_debug) {
    printf(">>> cgf_fin_ovf_match %04x.%04x\n", tilepath, tilestep);
  }
  */

  rec_a = 0;
  step_a = -1;
  while ((byte_offset_a < byte_len_a) && (step_a < tilestep) && (rec_a < n_a)) {

    knot_a[0].clear();
    knot_a[1].clear();

    if (code_a[rec_a]==0) {
      dn = cgf_final_overflow_map0_peel(map_a + byte_offset_a, &step_a, &k, knot_a);
      if (dn<=0) { return 0; }
      byte_offset_a += dn;

      if (k!=2) { return 0; }
    } else { return 0; }

    rec_a++;
  }

  /*
  if (local_debug) {
    printf(" cp0\n");
  }
  */

  rec_b=0;
  step_b = -1;
  while ((byte_offset_b < byte_len_b) && (step_b < tilestep) && (rec_b < n_b)) {

    knot_b[0].clear();
    knot_b[1].clear();


    if (code_b[rec_b]==0) {
      dn = cgf_final_overflow_map0_peel(map_b + byte_offset_b, &step_b, &k, knot_b);
      if (dn<=0) { return 0; }
      byte_offset_b += dn;

      if (k!=2) { return 0; }
    } else { return 0; }

    rec_b++;
  }

  /*
  if (local_debug) {
    printf("fin: %04x.%04x: fin ovf: rec_a %i, rec_b %i (step_a %x, step_b %x)\n", tilepath, tilestep, rec_a, rec_b, step_a, step_b);
  }
  */

  if (step_a!=step_b) { return 0; }

  /*
  if (local_debug) {
    printf("%04x.%04x a:", tilepath, tilestep);
    for (i=0; i<2; i++) {
      printf(" [");
      for (j=0; j<knot_a[i].size(); j++) printf(" %x", knot_a[i][j]);
      printf("]");
    }
    printf("\n");

    printf("%04x.%04x b:", tilepath, tilestep);
    for (i=0; i<2; i++) {
      printf(" [");
      for (j=0; j<knot_b[i].size(); j++) printf(" %x", knot_b[i][j]);
      printf("]");
    }
    printf("\n");

  }
  */

  for (i=0; i<2; i++) {
    if (knot_a[i].size() != knot_b[i].size()) { return 0; }
    for (j=0; j<knot_a[i].size(); j++) {
      if (knot_a[i][j] != knot_b[i][j]) { return 0; }
    }
  }

  /*
  if (local_debug) {
    printf("fin_ovf++ %04x.%04x\n", tilepath, tilestep);
  }
  */

  return 1;
}

// ovf_step has [ step , code a, code b ]
// where codeX is -1 for overflow, -2 for complex and has the code otherwise.
//
int cgf_overflow_concordance_2(int *n_match,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath,
    std::vector<int> &ovf_step) {
  int i, j, k, idx;
  int var_a, var_b;
  std::vector<int> fin_ovf_step;
  int match_count=0, fin_ovf_count=0;

  //int local_debug = 0;

  std::vector<int> steps;
  std::vector<int> varids_a, varids_b;

  for (i=0; i<ovf_step.size(); i+=3) { steps.push_back(ovf_step[i]); }

  cgf_map_variant_ids(cgf_a, tilepath, steps, varids_a);
  cgf_map_variant_ids(cgf_b, tilepath, steps, varids_b);

  for (idx=0; idx<steps.size(); idx++) {
    var_a = varids_a[idx];
    var_b = varids_b[idx];

    /*
    if (local_debug) {
      printf("ovf_conc_2: %04x.%04x var_a %x, var_b %x\n", tilepath, steps[idx], var_a, var_b);
    }
    */

    if ((var_a < 1024) && (var_b < 1024)) {

      if (var_a==var_b) {

        /*
        if (local_debug) {
          printf("mo: %04x.00.%04x\n", tilepath, steps[idx]);
        }
        */

        match_count++;
      }

    } else if ((var_a>1024) && (var_b>1024)) {

      // 1024 is a spanning tile,
      // 1025 is a final overflow
      //
      fin_ovf_step.push_back(steps[idx]);
      fin_ovf_count++;
    }

  }

  for (i=0; i<fin_ovf_step.size(); i++) {
    if (cgf_final_overflow_match(cgf_a, cgf_b, tilepath, fin_ovf_step[i])) {

      /*
      if (local_debug) {
        printf("mf: %04x.00.%04x\n", tilepath, fin_ovf_step[i]);
      }
      */

      match_count++;
    }
  }

  *n_match = match_count;

  return 0;


}

// ovf_step has [ step , code a, code b ]
// where codeX is -1 for overflow, -2 for complex and has the code otherwise.
//
int cgf_overflow_concordance(int *n_match,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath,
    std::vector<int> &ovf_step) {
  int i, j, k, idx;
  int var_a, var_b, step;
  std::vector<int> fin_ovf_step;
  int match_count=0, fin_ovf_count=0;

  //int local_debug = 0;

  for (idx=0; idx<ovf_step.size(); idx+=3) {
    step = ovf_step[idx];
    var_a = ovf_step[idx+1];
    var_b = ovf_step[idx+2];

    // complex, ignore
    //
    if ((var_a<-1) || (var_b<-1)) {

      /*
      if (local_debug) {
        printf("idx: %d, %x.%x, var_a %d, var_b %d, complex, ignoring\n",
            idx, tilepath, step, var_a, var_b);
      }
      */

      continue;
    }

    if (var_a<0) {
      var_a = cgf_map_variant_id(cgf_a, tilepath, step);
    }

    if (var_b<0) {
      var_b = cgf_map_variant_id(cgf_b, tilepath, step);
    }

    /*
    if (local_debug) {
      printf("%x.%x var_a %d, var_b %d\n", tilepath, step, var_a, var_b);
    }
    */

    if ((var_a < 1024) && (var_b < 1024)) {
      if (var_a==var_b) {

        /*
        if (local_debug) {
          printf("%04x.%04x, var_a %d, var_b %d, ovf_conf++\n", tilepath, step, var_a, var_b);
          printf("mo: %04x.00.%04x\n", tilepath, step);
        }
        */

        match_count++;
      }
    } else if ((var_a>1024) && (var_b>1024)) {

      // 1024 is a spanning tile, 1025 is a final overflow
      //
      fin_ovf_step.push_back(step);
      fin_ovf_count++;

      /*
      if (local_debug) {
        printf("%04x.%04x: fin_ovf queue\n", tilepath, step);
      }
      */
    }

  }

  for (i=0; i<fin_ovf_step.size(); i++) {

    if (cgf_final_overflow_match(cgf_a, cgf_b, tilepath, fin_ovf_step[i])) {

      /*
      if (local_debug) {
        printf("%04x.%04x: fin_ovf_count++\n", tilepath, fin_ovf_step[i]);
        printf("mf: %04x.00.%04x\n", tilepath, fin_ovf_step[i]);
      }
      */

      match_count++;
    }
  }

  *n_match = match_count;

  return 0;
}

uint8_t cgf_loq_tile(cgf_t *cgf, int tilepath, int tilestep) {
  return cgf->path[tilepath].loq_info->loq_flag[tilestep/8] & (1<<(tilestep%8));
}

// Only consider either canonical tiles,
// cached overflows or tile mapped overflows.
// All otherws (final overflows, low quality
// tiles, etc.) will be ignored.
//
int cgf_tile_concordance_2(int *n_match,
    int *n_loq,
    cgf_t *cgf_a, cgf_t *cgf_b,
    int tilepath, int start_step, int n_step) {

  int i, j, k, bit_idx;
  int start_block, end_block, s, s_beg, s_end;
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

  //int local_debug = 0;
  int loq_count=0;

  uint8_t *loq_flag_a, *loq_flag_b;
  uint8_t mask8;

  int ii;
  uint32_t debug32;
  uint64_t debug64;

  std::vector<int> ovf_info;

  path_a = &(cgf_a->path[tilepath]);
  path_b = &(cgf_b->path[tilepath]);

  s_beg = start_step/8;
  s_end = (start_step+n_step+7)/8;
  for (s=s_beg; s<s_end; s++) {
    mask8 = 0xff;
    if (s==s_beg) {
      mask8 = 0xff >> (start_step%8);
    }

    if (s==(s_end-1)) {
      k = (start_step + n_step)%8;
      mask8 &= 0xff << (7-k);
    }

    loq_count += NumberOfSetBits8(mask8 & (path_a->loq_info->loq_flag[s] | path_b->loq_info->loq_flag[s]));
  }
  *n_loq = loq_count;

  start_block = start_step / 32;
  end_block = (start_step + n_step) / 32;

  for (s=start_block; s<=end_block; s++) {

    mask = 0xffffffff00000000;
    skip_beg = 0;
    use_end = 32;

    if (s==start_block) {

      //lsb in upper 4 bytes is first entry
      //
      u32 = (((uint32_t)(0xffffffff)) << (start_step%32));
      mask &= (uint64_t)u32 << 32;

      skip_beg = start_step % 32;
    }

    if (s==end_block) {

      //lsb in upper 4 bytes is first entry
      //
      u32 = (((uint32_t)0xffffffff) >> (32-((start_step+n_step)%32)));
      mask &= (uint64_t)u32 << 32;

      use_end = (start_step + n_step) % 32;
    }

    // A little sloppy but skip this last block if
    // there are no bits to consider.
    //
    if (use_end==0) { continue; }

    x32 = ((path_a->vec[s] & mask ) >> 32);
    y32 = ((path_b->vec[s] & mask ) >> 32);
    k = NumberOfSetBits(x32 | y32);
    canon_match_count += (32-skip_beg-(32-use_end)) - k;

    /*
    if (local_debug) {
      debug32 = x32 | y32;
      for (ii=skip_beg; ii<use_end; ii++) {
        if ((debug32 & (1<<ii))==0) {

          if (s==end_block) {
            printf("?? s: %i, skip_beg %i, use_end %i, debug32 %08x mask %016" PRIx64 "\n", s, skip_beg, use_end, (unsigned int)debug32, mask);
          }

          printf("mc: %04x.00.%04x\n", tilepath, 32*s + ii);
        }
      }
    }
    */

    /*
    if (local_debug) {
      fullx32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      fully32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      printf(">> s: %i, k: %i, x32: %08x (%08x), y32: %08x (%08x), skip_beg: %d, use_end: %d, mask: %016" PRIx64 "\n",
          s, k,
          (unsigned int)x32, (unsigned int)fullx32,
          (unsigned int)y32, (unsigned int)fully32,
          skip_beg, use_end, mask);
    }
    */

    if (k>0) {

      // need full vector
      //
      x32 = ((path_a->vec[s] & 0xffffffff00000000 ) >> 32);
      y32 = ((path_b->vec[s] & 0xffffffff00000000 ) >> 32);

      hexit_a_n = NumberOfSetBits(x32);
      hexit_b_n = NumberOfSetBits(y32);

      lx32 = path_a->vec[s] & 0xffffffff;
      ly32 = path_b->vec[s] & 0xffffffff;

      //if (local_debug) { printf("  lx32: %08x, ly32: %08x\n", (unsigned int)lx32, (unsigned int)ly32); }

      for (i=0; i<8; i++) {
        hexit_a[i] = (uint8_t)((lx32 & (0xf << (4*i)))>>(4*i));
        hexit_b[i] = (uint8_t)((ly32 & (0xf << (4*i)))>>(4*i));
      }

      a_count=0;
      b_count=0;
      and32 = x32 & y32;

      for (i=0; i<32; i++) {
        bit_idx = i;

        /*
        if (local_debug) {
          printf("  [%i(%i)] (%c,%c:%c) a_count %i, b_count %i\n",
              i, bit_idx,
              (x32&(1<<i)) ? '*' : '_',
              (y32&(1<<i)) ? '*' : '_',
              (and32&(1<<i)) ? '*' : '_', a_count, b_count);
          if (and32 & (1<<i)) {
            if (a_count<8) { printf("    a[%i]: %x\n", a_count, hexit_a[a_count]); }
            if (b_count<8) { printf("    b[%i]: %x\n", b_count, hexit_b[b_count]); }
          }
        }
        */

        if (and32 & (1<<i)) {
          if ((a_count<8) && (b_count<8) &&
              (hexit_a[a_count] > 0) && (hexit_a[a_count] < 0xd) &&
              (hexit_b[b_count] > 0) && (hexit_b[b_count] < 0xd)) {

            if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
              cache_match_count += ((hexit_a[a_count] == hexit_b[b_count]) ? 1 : 0);

              /*
              if (local_debug) {
                if (hexit_a[a_count]==hexit_b[b_count]) {
                  printf("mh: %04x.00.%04x\n", tilepath, s*32 + i);
                }
              }
              */

              //if (local_debug) { printf("      cache_match_count%s\n", (hexit_a[a_count] == hexit_b[b_count]) ? "++" : ".." ); }
            }
            //else if (local_debug) { printf("      skipped (cache_match_count++)\n"); }

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

                  //if (local_debug) { printf("      loq_cache_count++\n"); }
                }
                //else if (local_debug) { printf("      skipped (loq_cache_count++)\n"); }

              }

              // Both are high quiality overflow
              //
              else if ( ((flag & (1<<1))>>1) & ((flag & (1<<4))>>4) ) {

                if ((bit_idx >= skip_beg) && (bit_idx < use_end)) {
                  ovf_count++;

                  // push step into vector for later processing
                  //
                  ovf_info.push_back(s*32 + bit_idx);
                  ovf_info.push_back(-1);
                  ovf_info.push_back(-1);

                  //if (local_debug) { printf("      ovf_count++ (hiq ovf)\n"); }
                }
                //else if (local_debug) { printf("      skipped (ovf_count++)\n"); }

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

                  /*
                  if (local_debug) {
                    int ii = ovf_info.size();
                    printf("        cache_ovf_count++ (a) [%d: %d %d]\n", ovf_info[ii-3], ovf_info[ii-2], ovf_info[ii-1]);
                  }
                  */

                }

                /*
                else if (local_debug) {
                  printf("      skipped (step %d %x) (cache_ovf_count++) loq tile %d %d (a)\n",
                      s*32 + bit_idx, s*32 + bit_idx,
                      cgf_loq_tile(cgf_a, tilepath, s*32 + bit_idx),
                      cgf_loq_tile(cgf_b, tilepath, s*32 + bit_idx)
                      );
                }
                */

              }
              //else if (local_debug) { printf("      skipped (step %d %x) (cache_ovf_count++) (b)\n", s*32 + bit_idx, s*32 + bit_idx); }

            }

          }

        }

        if (x32 & (1<<i)) { a_count++; }
        if (y32 & (1<<i)) { b_count++; }

      }

    }

  }

  /*
  if (local_debug) {
    for (i=0; i<ovf_info.size(); i++) { printf("ovf_info[%i]: %x\n", i, ovf_info[i]); }
  }
  */

  //cgf_overflow_concordance(&k, cgf_a, cgf_b, tilepath, ovf_info);
  cgf_overflow_concordance_2(&k, cgf_a, cgf_b, tilepath, ovf_info);

  /*
  if (local_debug) {
    printf(">>>> overflow match %d\n", k);
    printf(">>>> canon match %i, cache_match %i, overflow %i\n", canon_match_count, cache_match_count, k);
  }
  */

  *n_match = canon_match_count + cache_match_count + k;

  return 0;
}

//----

// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX

int cgf_loq_count(cgf_t *cgf, int tilepath, int start_tilestep, int n_tilestep) {
  int i, j, k;
  int beg_block, end_block;
  uint8_t u8, mask;
  int loq_count=0;

  beg_block = start_tilestep/8;
  end_block = (start_tilestep + n_tilestep)/8;

  u8 = cgf->path[tilepath].loq_info->loq_flag[beg_block];

  mask = (uint8_t)(((uint8_t)0xff) << (start_tilestep%8) );

  if (end_block == beg_block) {
    k = ((start_tilestep + n_tilestep)%8);
    mask &= ((uint8_t)0xff) >> (8-k);
  }

  loq_count += NumberOfSetBits8( u8 & mask );

  if (end_block > beg_block) { mask = 0xff; }

  for (i=(beg_block+1); i<end_block; i++) {
    loq_count += NumberOfSetBits8( cgf->path[tilepath].loq_info->loq_flag[i] );
  }

  if ( (end_block > beg_block) &&
      (((start_tilestep + n_tilestep)%8) > 0) ) {
    k = ((start_tilestep + n_tilestep)%8);
    mask &= ((uint8_t)0xff) >> (8-k);

    u8 = cgf->path[tilepath].loq_info->loq_flag[end_block];
    loq_count += NumberOfSetBits8( u8 & mask );
  }

  return loq_count;
}

int cgf_loq_block(cgf_t *cgf, int tilepath, int tilestep) {
  int b;
  uint64_t stride=0, byte_offset, n_loq_block;
  cgf_low_quality_info_t *loq_info;

  loq_info = cgf->path[tilepath].loq_info;

  stride = loq_info->stride;
  if (stride==0) { return -1; }
  n_loq_block = (loq_info->count + stride - 1) / stride;

  for (b=1; b<n_loq_block; b++) {
    if (tilestep < loq_info->step_position[b]) { break; }
  }
  b--;

  return b;
}

int cgf_loq_offset_2(cgf_t *cgf, int tilepath, int tilestep) {
  int i, j, k;
  int b, n_loq_block = -1;
  uint64_t stride=0, byte_offset;
  int block_startstep;
  int loq_count=0;
  cgf_low_quality_info_t *loq_info;

  loq_info = cgf->path[tilepath].loq_info;

  b = cgf_loq_block(cgf, tilepath, tilestep);

  printf("  b %i\n", b);

  byte_offset = loq_info->offset[b];
  block_startstep = (int)loq_info->step_position[b];

  printf("  byte_offset %i, block_startstep: %i\n", (int)byte_offset, (int)block_startstep);

  if (tilestep < block_startstep) { return 0; }

  printf("  dn: %i\n", tilestep - (int)block_startstep);

  loq_count = cgf_loq_count(cgf, tilepath, block_startstep, tilestep - (int)block_startstep);
  return loq_count;
}

int cgf_loq_offset(cgf_t *cgf, int tilepath, int tilestep) {
  int i;
  int q, r;
  int loq_count=0;

  q = tilestep / 8;
  r = tilestep % 8;

  for (i=0; i<q; i++) {
    loq_count += NumberOfSetBits8(cgf->path[tilepath].loq_info->loq_flag[i]);
  }

  for (i=0; i<r; i++) {
    if (cgf->path[tilepath].loq_info->loq_flag[q] & (1<<i)) { loq_count++; }
  }

  return loq_count;
}

uint8_t cgf_loq_is_hom(cgf_t *cgf, int tilepath, int loq_idx) {
  return cgf->path[tilepath].loq_info->hom_flag[loq_idx/8] & (1<<(loq_idx%8));
}


// This does not fill out spanning tile information with empty low quality tile information.
// v is a two element array of vector of vectors, with each base vector holding an interleaved
//   (start,length) tuple of low quality information.
//
int cgf_expand_loq_info(cgf_t *cgf, int tilepath, int tilestep, std::vector< std::vector<int> > *v) {
  int i, j, k, dn;
  int loq_offset=0;
  uint8_t u8;
  int block, loq_rel_count;
  uint64_t block_startstep;
  uint64_t byte_offset;
  uint64_t n_byte;
  cgf_low_quality_info_t *loq_info;
  uint8_t hom_flag;
  uint8_t *loq_bytes;

  uint32_t ntile[2], loq_ent_len, delpos, loqlen;
  int a, cur_loq_idx;
  int prev_off=0;

  //int local_debug=0;
  std::vector<int> tv;

  v[0].clear();
  v[1].clear();

  loq_info = cgf->path[tilepath].loq_info;
  loq_bytes = loq_info->loq_info;

  block = cgf_loq_block(cgf, tilepath, tilestep);

  byte_offset = loq_info->offset[block];
  block_startstep = loq_info->step_position[block];

  if (!cgf_loq_tile(cgf, tilepath, tilestep)) {

    //DEBUG
    /*
    if (local_debug) {
      printf("not a loq tile\n");
    }
    */

    tv.clear();
    v[0].push_back(tv);
    v[1].push_back(tv);

    return -1;
  }

  //DEBUG
  /*
  if (local_debug) {
    printf("  block %i, byte_offset %i, block_startstep %i\n", (int)block, (int)byte_offset, (int)block_startstep);
  }
  */

  loq_rel_count = cgf_loq_count(cgf, tilepath, block_startstep, tilestep - (int)block_startstep);

  n_byte = loq_info->loq_info_byte_count;

  /*
  if (local_debug) {
    printf(">>>> loq_rel_count %i (block_startstep %i, tilestep - block_startstep %i), n_byte %i\n",
        (int)loq_rel_count,
        (int)block_startstep,
        tilestep - (int)block_startstep,
        (int)n_byte);
  }
  */

  cur_loq_idx = 0;
  while ((byte_offset < n_byte) && (cur_loq_idx <= loq_rel_count)) {

    hom_flag = cgf_loq_is_hom(cgf, tilepath, (loq_info->stride)*block + cur_loq_idx);

    //DEBUG
    /*
    if (local_debug) {
      printf("byte_offset %i (%i), cur_loq_idx %i, loq_rel_count %i, hom %02x\n", (int)byte_offset, (int)n_byte, (int)cur_loq_idx, (int)loq_rel_count, hom_flag);
    }
    */

    if (hom_flag) {

      dn = dlug_convert_uint32(loq_bytes + byte_offset, &(ntile[0]));
      if (dn<0) { return -1; }
      byte_offset += dn;

      //DEBUG
      //if (local_debug) { printf("hom:"); }

      for (i=0; i<ntile[0]; i++) {

        if (cur_loq_idx == loq_rel_count) {
          tv.clear();
          v[0].push_back(tv);
          v[1].push_back(tv);
        }

        dn = dlug_convert_uint32(loq_bytes + byte_offset, &loq_ent_len);
        if (dn<0) { return -1; }
        byte_offset += dn;

        //DEBUG
        //if (local_debug) { printf("["); }

        prev_off = 0;
        for (j=0; j<loq_ent_len; j+=2) {
          dn = dlug_convert_uint32(loq_bytes + byte_offset, &delpos);
          if (dn<0) { return -1; }
          byte_offset += dn;

          dn = dlug_convert_uint32(loq_bytes + byte_offset, &loqlen);
          if (dn<0) { return -1; }
          byte_offset += dn;

          if (cur_loq_idx == loq_rel_count) {
            v[0][i].push_back(((int)delpos) + prev_off);
            v[0][i].push_back((int)loqlen);
            v[1][i].push_back(((int)delpos) + prev_off);
            v[1][i].push_back((int)loqlen);
          }

          //DEBUG
          //if (local_debug) { printf(" %i+%i", ((int)delpos)+prev_off, (int)loqlen); }

          prev_off += (int)delpos;

        }

        //DEBUG
        //if (local_debug) { printf(" ]"); }

      }

      //DEBUG
      //if (local_debug) { printf("\n"); }

    } else {

      dn = dlug_convert_uint32(loq_bytes + byte_offset, &(ntile[0]));
      if (dn<0) { return -1; }
      byte_offset += dn;

      dn = dlug_convert_uint32(loq_bytes + byte_offset, &(ntile[1]));
      if (dn<0) { return -1; }
      byte_offset += dn;

      //DEBUG
      //if (local_debug) { printf("het:"); }

      for (a=0; a<2; a++) {

        //DEBUG
        //if (local_debug) { printf("\n  "); }

        for (i=0; i<ntile[a]; i++) {

          if (cur_loq_idx == loq_rel_count) {
            tv.clear();
            v[a].push_back(tv);
          }


          dn = dlug_convert_uint32(loq_bytes + byte_offset, &loq_ent_len);
          if (dn<0) { return -1; }
          byte_offset += dn;

          //DEBUG
          //if (local_debug) { printf("["); }

          prev_off = 0;
          for (j=0; j<loq_ent_len; j+=2) {
            dn = dlug_convert_uint32(loq_bytes + byte_offset, &delpos);
            if (dn<0) { return -1; }
            byte_offset += dn;

            dn = dlug_convert_uint32(loq_bytes + byte_offset, &loqlen);
            if (dn<0) { return -1; }
            byte_offset += dn;

            if (cur_loq_idx == loq_rel_count) {
              v[a][i].push_back(((int)delpos) + prev_off);
              v[a][i].push_back((int)loqlen);
            }

            //DEBUG
            //if (local_debug) { printf(" %i+%i", ((int)delpos) + prev_off, (int)loqlen); }

            prev_off += (int)delpos;
          }

          //DEBUG
          //if (local_debug) { printf(" ]"); }

        }

      }

      //DEBUG
      //if (local_debug) { printf("\n"); }

    }

    cur_loq_idx++;


  }

  //if (local_debug) { printf("!!!!\n"); }


  /*
  u8 = cgf_loq_tile(cgf, tilepath, tilestep);

  //DEBUG
  printf("??? %04x\n", u8);
  printf("    %04x %04x %04x\n",
      cgf->path[tilepath].loq_info->loq_flag[tilestep/8],
      1<<(tilestep%8),
      cgf->path[tilepath].loq_info->loq_flag[tilestep/8] & (1<<(tilestep%8)));
      */

  if (cgf_loq_tile(cgf, tilepath, tilestep)==0) { return -1; }

  loq_offset = cgf_loq_offset(cgf, tilepath, tilestep);

  //if (local_debug) { printf(">>> %x.%x: loq_offset %i\n", tilepath, tilestep, loq_offset); }

  return 0;
}

// Fill in loq_info with low quality information about each tile.
// The base vector holds interleaved start position and length information
// of the low quality tile.  For example:
//
//   [ [ [ 3, 5, 16, 1 ], [] ], [ [ 3, 5, 15, 2 ], [3,1] ] ]
//
// would indicate diploid entry with two tiles in each allele,
// with the first having a nocall starting at 3 of 5 long, next
// starting at 16 with 1 long and the second tile on the first
// allele without any nocalls.  The second allele would have a nocall
// on the first tile starting at 3 of 5 long, next starting at 15
// with 2 long and the second tile on the second allele starting at 3 of
// length 1.
//
// It is the callers responsibility to make sure the allele holds valid
// information and that the first entry in each of the alleles is a non-spanning
// tile.  If filtering needs to be done it should be done after the fact (after
// the appopriate entries have been filled in here).
//
int cgf_loq_tile_band(cgf_t *cgf,
    int tilepath, int tilestep_beg, int tilestep_n,
    std::vector<int> *allele,
    std::vector< std::vector<int> > *loq_info) {
  int i, j, k;
  int s, s_end;
  int tilemap_id;
  int **tilemap_entry;
  int val, span, knot_span[2];
  int a, aa, curstep;

  //int add_empty_loq = 0;

  int local_debug = 0;
  int loq_step_pos[2];
  int step_idx;
  std::vector<int> knot[2];
  std::vector<int> t;
  std::vector< std::vector<int> > loqv[2];

  loq_info[0].clear();
  loq_info[1].clear();

  loq_step_pos[0] = tilestep_beg;
  loq_step_pos[1] = tilestep_beg;
  step_idx = 0;

  while (step_idx < tilestep_n) {
    loqv[0].clear();
    loqv[1].clear();

    // It's a high quality tile, consider it as a special case.
    // Fill in the appropriate empty low quality vectors based
    // on the 'knot'.
    //
    if (cgf_loq_tile(cgf, tilepath, tilestep_beg + step_idx)==0) {
      t.clear();

      do {
        loq_info[0].push_back(t);
        loq_info[1].push_back(t);
        step_idx++;
      } while ((step_idx < tilestep_n) &&
             ( (allele[0][tilestep_beg+step_idx]<0) || (allele[1][tilestep_beg+step_idx]<0) ) );
      continue;
    }

    // store loq info in loqv
    //
    k = cgf_expand_loq_info(cgf, tilepath, tilestep_beg + step_idx, loqv);

    //DEBUG
    //DEBUG
    if (local_debug) {
      printf(">> loqv %i %i, step_idx %i (%i)\n", (int)loqv[0].size(), (int)loqv[1].size(), step_idx, tilestep_n);
      for (i=0; i<loqv[0].size(); i++) {
        printf(" [");
        for (j=0; j<loqv[0][i].size(); j++) {
          printf(" %i", loqv[0][i][j]);
        }
        printf("]");
      }
      printf("\n");
      for (i=0; i<loqv[1].size(); i++) {
        printf(" [");
        for (j=0; j<loqv[1][i].size(); j++) {
          printf(" %i", loqv[1][i][j]);
        }
        printf("]");
      }
      printf("\n");
    }
    //DEBUG
    //DEBUG


    //add_empty_loq = ((k<0) ? 1 : 0);
    //add_empty_loq = 0;


    //if (!add_empty_loq) {

      // add to loq_info
      //
      for (a=0; a<2; a++) {
        int cur_idx = 0;

        for (i=0; i<loqv[a].size(); i++) {

          if ((step_idx + cur_idx) < tilestep_n) {
            loq_info[a].push_back(loqv[a][i]);
          }

          cur_idx++;
          while (((step_idx + cur_idx) < tilestep_n) &&
                 (allele[a][step_idx+cur_idx]<0)) {
            cur_idx++;
            t.clear();
            loq_info[a].push_back(t);
          }
        }

        /*
        // Fill in remaining loq element positions with empty entries
        //
        while ((cur_idx < tilestep_n) && (allele[a][cur_idx]<0)) {
          t.clear();
          loq_info[a].push_back(t);
          cur_idx++;
        }
        */

      }

    //}

    // skip to next 'knot' (skip over spanning tiles until
    // we reach next anchor tile)
    //
    t.clear();
    do {
      step_idx++;

      //if (add_empty_loq) {
      /*
        if (step_idx < tilestep_n) {
          loq_info[0].push_back(t);
          loq_info[1].push_back(t);
        }
        */
      //}

    } while ((step_idx < tilestep_n) &&
        ((allele[0][step_idx] < 0) || (allele[1][step_idx] < 0)) );

  }

  /*
  for (aa=0; aa<2; aa++) {
    for (curstep=tilestep_beg; curstep<(tilestep_beg + tilestep_n); ) {

      if (allele[aa][curstep]<0) { return -1; }

      t.clear();
      loq_info[aa].push_back(t);

      if (cgf_loq_tile(cgf, tilepath, curstep)!=0) {
        // find loq information and add it to current loq entry
      }

      t.clear();
      for (curstep++; curstep<allele[aa].size(); curstep++) {
        if (allele[aa][curstep] >= 0) { break; }
        loq_info[aa].push_back(t);
      }

    }
  }
  */

  return 0;
}


int cgf_tile_band(cgf_t *cgf,
    int tilepath, int tilestep_beg, int tilestep_n,
    std::vector<int> *allele) {
  int i, j, k;
  int s, s_end;
  int tilemap_id;
  int **tilemap_entry;
  int val, span, knot_span[2];

  int tilestep_beg_actual, tilestep_n_actual;

  int local_debug = 0;
  std::vector<int> knot[2];

  allele[0].clear();
  allele[1].clear();

  s = tilestep_beg;
  s_end = tilestep_beg + tilestep_n;

  tilestep_beg_actual = tilestep_beg;
  tilestep_n_actual = tilestep_n;

  tilemap_id = cgf_map_variant_id(cgf, tilepath, tilestep_beg);

  if (local_debug) {
    if (tilemap_id<0) {
      printf("cgf_tile_band: start on spanning? (tilemap id %i), tilemap_beg (%i)\n",
          tilemap_id, tilestep_beg);
    }
  }


  if (tilemap_id<0) {
    //for (; (tilemap_id<0) && (tilestep_beg<s_end); tilestep_beg++) {
    while ((tilemap_id<0) && (tilestep_beg>0)) {
      tilestep_beg--;
      tilemap_id = cgf_map_variant_id(cgf, tilepath, tilestep_beg);

      if (local_debug) {
        printf("cgf_tile_band: start on spanning? (tilemap id %i), tilemap_beg (%i)\n",
            tilemap_id, tilestep_beg);
      }

    }

  }

  //DEBUG
  tilestep_beg_actual = tilestep_beg;

  if (local_debug) {
    printf("cgf_tile_band: start tilemap_beg (%i), n %i (end %i)\n",
            tilestep_beg, tilestep_n, s_end);
  }

  if (tilestep_beg==s_end) { return -1; }

  int del_s = 0;

  for (s=tilestep_beg; s<s_end; ) {
    tilemap_id = cgf_map_variant_id(cgf, tilepath, s);

    if (local_debug) {
      if (tilemap_id==-1) {
        printf("TILEMAP_ID -1? %i, tilepath %i, s %i\n", tilemap_id, tilepath, s);
      }
      printf("%04x.%04x tilemap_id %i\n", tilepath, s, tilemap_id);
    }

    if (tilemap_id<0) { return -3; }

    knot_span[0] = 0;
    knot_span[1] = 0;


    if ((tilemap_id>=0) && (tilemap_id<1024)) {

      tilemap_entry = cgf->tile_map[tilemap_id];

      del_s = 0;
      for (i=0; i<tilemap_entry[0][0]; i++) {
        val = tilemap_entry[0][2*i+1];
        span = tilemap_entry[0][2*i+2];

        knot_span[0]+=span;

        //if ((s + del_s) >= tilestep_beg_actual) {
        if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
          allele[0].push_back(val);
        }
        del_s++;
        for (j=1; j<span; j++) {
          //if ((s + del_s) >= tilestep_beg_actual) {
          if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
            allele[0].push_back(-1);
          }
          del_s++;
        }
      }

      del_s = 0;
      for (i=0; i<tilemap_entry[1][0]; i++) {
        val = tilemap_entry[1][2*i+1];
        span = tilemap_entry[1][2*i+2];

        knot_span[1]+=span;

        //if ((s + del_s) >= tilestep_beg_actual) {
        if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
          allele[1].push_back(val);
        }
        del_s++;
        for (j=1; j<span; j++) {
          //if ((s + del_s) >= tilestep_beg_actual) {
          if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
            allele[1].push_back(-1);
          }
          del_s++;
        }
      }

      if (local_debug) {
        printf("knot_span (%x.%x) tilemap_id:%i, %i %i\n", tilepath, s, tilemap_id, knot_span[0], knot_span[1]);
      }

      if (knot_span[0] != knot_span[1]) { return -1; }
      if (knot_span[0] <= 0) { return -2 ; }
      s += knot_span[0];

    }
    else {
      //DEBUG
      if (local_debug) {
        printf("adding step %i val %i\n", s, tilemap_id);
      }

      k = cgf_final_overflow_knot(cgf, tilepath, s, knot);

      knot_span[0] = 0;
      knot_span[1] = 0;

      if (local_debug) {
        printf("k %i\n", k);

        printf(">>> knot[0]:");
        for (i=0; i<knot[0].size(); i++) printf(" %i", knot[0][i]);
        printf("\n");

        printf(">>> knot[1]:");
        for (i=0; i<knot[1].size(); i++) printf(" %i", knot[1][i]);
        printf("\n");
      }

      del_s=0;
      for (i=0; i<knot[0].size(); i+=2) {
        if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
          allele[0].push_back(knot[0][i]);
        }
        del_s++;
        knot_span[0] += knot[0][i+1];
        for (j=1; j<knot[0][i + 1]; j++) {
          //if ((s + del_s) >= tilestep_beg_actual) {
          if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
            allele[0].push_back(-1);
          }
          del_s++;
        }
      }

      del_s=0;
      for (i=0; i<knot[1].size(); i+=2) {
        //if ((s + del_s) >= tilestep_beg_actual) {
        if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
          allele[1].push_back(knot[1][i]);
        }
        del_s++;
        knot_span[1] += knot[1][i+1];
        for (j=1; j<knot[1][i + 1]; j++) {
          //if ((s + del_s) >= tilestep_beg_actual) {
          if (((s + del_s) >= tilestep_beg_actual) && ((s+del_s)<s_end)) {
            allele[1].push_back(-1);
          }
          del_s++;
        }
      }

      //allele[0].push_back(tilemap_id);
      //allele[1].push_back(tilemap_id);

      if (knot_span[0] != knot_span[1]) { return -1; }
      if (knot_span[0] <= 0) { return -2 ; }
      s += knot_span[0];


      //return -1;
    }

  }

  return 0;
}

// Return byte offset of tilestep in tilepath final overflow data bytes,
// -1 on step not found
//
int cgf_final_overflow_step_offset(cgf_t *cgf, int tilepath, int tilestep) {
  uint64_t prev_byte_offset, byte_offset, byte_len, n_rec, data_byte_len;
  int n, dn;
  int cur_step, n_allele, rec;
  uint8_t *code, *data;
  cgf_final_overflow_t *fin_ovf;

  fin_ovf = cgf->path[tilepath].final_overflow;
  byte_len = fin_ovf->data_record_byte_len;
  n_rec = fin_ovf->data_record_n;

  code = fin_ovf->data_record->code;
  data = fin_ovf->data_record->data;

  data_byte_len = byte_len - n_rec;
  rec=0;

  byte_offset=0;
  prev_byte_offset=0;

  while ((byte_offset < data_byte_len) && (cur_step < tilestep)) {

    prev_byte_offset = byte_offset;

    if (code[rec]!=0) { return -3; }

    dn = cgf_final_overflow_map0_peel(data+byte_offset, &cur_step, &n_allele, NULL);
    if (dn<=0) { return -2; }
    byte_offset += (uint64_t)dn;

  }

  if (cur_step!=tilestep) { return -1; }

  //return (int)byte_offset;
  return (int)prev_byte_offset;

}

void test_lvl2(cgf_t *cgf, cgf_t *cgf_b) {
  int i, j, k;
  int pt = 0x9e;
  int st = 0x64d;
  int varid;
  int ofst;
  uint8_t *xbuf;
  int lvl=2;
  int n_loq;
  int n_match;

  int anchor_step=-1, n_allele=-1;
  std::vector<int> allele[2];

  for (i=0; i<20; i++) {
    varid = cgf_map_variant_id(cgf, pt, st+i);
    printf("a: %04x.%04x varid %x\n", pt, st+i, varid);

    if (varid==1024) {
      printf("spanning tile...?\n");
    }
    else if (varid > 1024) {

      printf(">>>>> varid > 1024 (%d), step %x\n", varid, st+i);

      allele[0].clear();
      allele[1].clear();

      ofst = cgf_final_overflow_step_offset(cgf, pt, st+i);

      printf(" got ofst: %i\n", ofst);

      if (ofst<0) { printf("NO\n"); exit(1); }

      xbuf = cgf->path[pt].final_overflow->data_record->data + ofst;

      printf(".... %x %x %x %x %x %x\n", xbuf[0], xbuf[1], xbuf[2], xbuf[3], xbuf[4], xbuf[5]);

      cgf_final_overflow_map0_peel(cgf->path[pt].final_overflow->data_record->data + ofst, &anchor_step, &n_allele, allele);

      printf("  >>> anchor_step %x, n_allele %i:", anchor_step, n_allele);
      for (int ii=0; ii<2; ii++) {
        printf("[%i](", ii);
        for (j=0; j<allele[ii].size(); j++) {
          printf(" %x", allele[ii][j]);
        }
        printf(") ");
      }
      printf("\n");

    }

  }

  for (i=0; i<20; i++) {
    varid = cgf_map_variant_id(cgf_b, pt, st+i);
    printf("b: %04x.%04x varid %x\n", pt, st+i, varid);

    if (varid==1024) {
      printf("spanning tile...?\n");
    }
    else if (varid >= 1024) {
      allele[0].clear();
      allele[1].clear();

      ofst = cgf_final_overflow_step_offset(cgf, pt, st+i);

      printf(" got ofst: %i\n", ofst);

      if (ofst<0) { printf("NO\n"); exit(1); }

      cgf_final_overflow_map0_peel(cgf->path[pt].final_overflow->data_record->data + ofst, &anchor_step, &n_allele, allele);

      printf("  >>> anchor_step %x, n_allele %i:", anchor_step, n_allele);

      for (int ii=0; ii<2; ii++) {
        printf("[%i](", ii);
        for (j=0; j<allele[ii].size(); j++) {
          printf(" %x", allele[ii][j]);
        }
        printf(") ");
      }
      printf("\n");


    }

  }

  printf("\n\n\n");

  //DEBUG
  i=0x9e;
  j=0;
  k=0;

  // test cgf_relative_overflow_count
  for (k=0; k<100; k++) {
    printf("%04x.%04x: a: %i\n",
        i, k,
        cgf_relative_overflow_count(cgf->path[i].vec, 0, k));

    varid = cgf_map_variant_id(cgf, i, k);
    printf(">>>>>>>>>>>>>> %04x.%04x: varid %x\n", i, k, varid);
  }

  printf("\n\n\n===========================\n\n\n");

  for (k=0; k<100; k++) {
    printf("%04x.%04x: b: %i\n",
        i, k,
        cgf_relative_overflow_count(cgf_b->path[i].vec, 0, k));

    varid = cgf_map_variant_id(cgf_b, i, k);
    printf(">>>>>>>>>>>>>> %04x.%04x: varid %x\n", i, k, varid);
  }

  printf("\n\n\n===========================\n\n\n");

  exit(1);


  cgf_tile_concordance_2(&n_match, &n_loq,
      cgf, cgf_b,
      i, 0, cgf->path[i].n_tile);
  printf("[%x] level: %i, matched %d (loq %d)\n", i, lvl, n_match, n_loq);
  k+=n_match;
  j+=n_loq;

  printf("level: %i, match: %i, loq: %d\n", lvl, k, j);
}


