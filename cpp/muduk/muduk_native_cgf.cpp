#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include <vector>
#include <string>

#include "duktape.h"

#include "muduk.h"

#include "cgb.hpp"

duk_ret_t muduk_native_tile_strip(duk_context *ctx) {
  int idx;
  double d;
  int n_match=-1, n_loq=-1;
  int tilepath, tile_begstep, tile_nstep;
  char *tbuf;

  d = duk_require_number(ctx, 0);
  idx = (int)d;

  tilepath = (int)duk_require_number(ctx, 1);
  tile_begstep = (int)duk_require_number(ctx, 2);
  tile_nstep = (int)duk_require_number(ctx, 3);

}


duk_ret_t muduk_native_tile_pair_concordance(duk_context *ctx) {
  int k;
  int idx0, idx1;
  double d_a, d_b;
  int n_match=-1, n_loq=-1, n_ovf=-1;
  int tilepath, tile_begstep, tile_nstep;
  char *tbuf;
  int lvl=2;

  int local_verbose = 0;

  cgf_t *cgf_canon;

  //DEBUG
  tilepath= 0x93;
  tile_begstep = 0;
  tile_nstep= 500;

  idx0 = (int)duk_require_number(ctx, 0);
  idx1 = (int)duk_require_number(ctx, 1);
  tilepath = (int)duk_require_number(ctx, 2);
  tile_begstep = (int)duk_require_number(ctx, 3);
  tile_nstep = (int)duk_require_number(ctx, 4);
  lvl = (int)duk_require_number(ctx, 5);

  cgf_canon = glob_ctx.cgf[0];

  if ((idx0<0) || (idx0>=glob_ctx.cgf.size()) ||
      (idx1<0) || (idx1>=glob_ctx.cgf.size()) ||
      (tilepath<0) || (tilepath>=cgf_canon->path_count) ||
      (tile_begstep<0) || (tile_begstep>=cgf_canon->step_per_path[tilepath]) ||
      (tile_nstep<0) || ((tile_begstep+tile_nstep)>cgf_canon->step_per_path[tilepath]) ||
      (lvl<0) || (lvl>2)) {

    printf("idx0 %i, idx1 %i, (%i) tilepath %i (%i), tile_begstep %i (%i), tile_nstep %i (%i)\n",
        idx0, idx1, (int)glob_ctx.cgf.size(),
        tilepath, (int)cgf_canon->path_count,
        tile_begstep, (int)cgf_canon->step_per_path[tilepath],
        tile_nstep,
        (int)(cgf_canon->step_per_path[tilepath]) - tile_begstep);

    printf("oob error\n");

    return -1;
  }

  if (local_verbose) {
    printf("%s:%s %04x.00.%04x+%x\n",
        glob_ctx.cgf_name[idx0].c_str(),
        glob_ctx.cgf_name[idx1].c_str(),
        tilepath, tile_begstep, tile_nstep);
  }


  if (lvl==2) {
    cgf_tile_concordance_2(&n_match, &n_loq,
        glob_ctx.cgf[idx0],
        glob_ctx.cgf[idx1],
        tilepath, tile_begstep, tile_nstep);
  }
  else if (lvl==1) {
    cgf_tile_concordance_1(&n_match, &n_ovf,
        glob_ctx.cgf[idx0],
        glob_ctx.cgf[idx1],
        tilepath, tile_begstep, tile_nstep);
  }
  else if (lvl==0) {
    cgf_tile_concordance_0(&n_match,
        glob_ctx.cgf[idx0],
        glob_ctx.cgf[idx1],
        tilepath, tile_begstep, tile_nstep);
  }

  if (local_verbose) {
    printf("  got: %i %i\n", n_match, n_loq);
  }

  k = asprintf(&tbuf, "{\"result\":\"ok\",\"match\":%i,\"low_quality\":%i,\"n_overflow\":%i}",
      n_match, n_loq, n_ovf);
  if (k<0) { return -1; }
  duk_push_string(ctx, tbuf);
  free(tbuf);

  return 1;
}


