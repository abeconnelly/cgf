#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include <vector>
#include <string>

#include "duktape.h"

#include "muduk.h"

#include "cgb.hpp"

duk_ret_t muduk_native_tile_band(duk_context *ctx) {
  int idx, i, j, k;
  double d;
  int n_match=-1, n_loq=-1;
  int tilepath, tile_begstep, tile_nstep;
  char *tbuf;
  char sbuf[1024];
  int loq_flag = 1;
  int n_tilestep_actual;


  std::vector<int> allele[2];
  std::vector< std::vector<int> > loq_allele[2];

  std::string outs;

  int backup_step = 0;
  cgf_t *cgf;

  idx = (int)duk_require_number(ctx, 0);
  tilepath = (int)duk_require_number(ctx, 1);
  tile_begstep = (int)duk_require_number(ctx, 2);
  tile_nstep = (int)duk_require_number(ctx, 3);

  cgf = glob_ctx.cgf[idx];

  n_tilestep_actual = tile_nstep;
  if ((tile_nstep<0) || ((tile_begstep+tile_nstep) > cgf->step_per_path[tilepath])) {
    n_tilestep_actual = cgf->step_per_path[tilepath] - tile_begstep;
  }


  cgf_tile_band(cgf, tilepath, tile_begstep, tile_nstep, allele);

  if (loq_flag) {

    if (allele[0].size() > n_tilestep_actual) {
      backup_step = allele[0].size() - n_tilestep_actual;
    }

    cgf_loq_tile_band(cgf, tilepath, tile_begstep-backup_step, n_tilestep_actual+backup_step, allele, loq_allele);
  }


  snprintf(sbuf,1024,"{");
  outs += sbuf;
  snprintf(sbuf,1024,"\"%04x\":{\n", tilepath);
  outs += sbuf;
  snprintf(sbuf,1024,"\"tilepath\":%i,", tilepath);
  outs += sbuf;
  snprintf(sbuf,1024,"\"start_tilestep\":%i,", tile_begstep);
  outs += sbuf;
  snprintf(sbuf,1024,"\"allele\":[\n");
  outs += sbuf;
  for (i=0; i<2; i++) {
    snprintf(sbuf,1024,"[");
    outs += sbuf;
    //for (j=0; j<allele[i].size(); j++) {
    for (j=0; j<(allele[i].size()-backup_step); j++) {
      int ele = j+backup_step;
      if (j>0) {
        snprintf(sbuf,1024,",");
        outs += sbuf;
      }
      //if ((j>0) && ((j%fold_w)==0)) { snprintf(sbuf,1024,"\n      "); }

      snprintf(sbuf,1024,"%i", allele[i][ele]);
      outs += sbuf;
    }
    snprintf(sbuf,1024,"]");
    outs += sbuf;
    if (i<(2-1)) {
      snprintf(sbuf,1024,",");
      outs += sbuf;
    }
    //else { snprintf(sbuf,1024,""); }
  }
  snprintf(sbuf,1024,"]");
  outs += sbuf;

  if (!loq_flag) {
    //snprintf(sbuf,1024,"\n");
  } else if (loq_flag) {
    snprintf(sbuf,1024,",");
    outs += sbuf;
    snprintf(sbuf,1024,"\"loq_info\":[");
    outs += sbuf;

    for (i=0; i<2; i++) {
      snprintf(sbuf,1024,"[");
      outs += sbuf;
      //for (j=0; j<loq_allele[i].size(); j++) {
      for (j=0; j<(loq_allele[i].size()-backup_step); j++) {
        int ele = j+backup_step;
        if (j>0) {
          snprintf(sbuf,1024,",");
          outs += sbuf;
        }
        //if ((j>0) && ((j%fold_w)==0)) { snprintf(sbuf,1024,""); }

        snprintf(sbuf,1024,"[");
        outs += sbuf;
        for (k=0; k<loq_allele[i][ele].size(); k++) {
          if (k>0) {
            snprintf(sbuf,1024,",");
            outs += sbuf;
          }
          snprintf(sbuf,1024,"%i", loq_allele[i][ele][k]);
          outs += sbuf;
        }
        snprintf(sbuf,1024,"]");
        outs += sbuf;

      }
      snprintf(sbuf,1024,"]");
      outs += sbuf;
      if (i<(2-1)) {
        snprintf(sbuf,1024,",");
        outs += sbuf;
      }
      //else { snprintf(sbuf,1024,"\n"); }
    }
    snprintf(sbuf,1024,"]");
    outs += sbuf;
  }

  snprintf(sbuf,1024,"}");
  outs += sbuf;

  snprintf(sbuf,1024,"}");
  outs += sbuf;

  duk_push_string(ctx, outs.c_str());

  return 1;

  /*

  k = asprintf(&tbuf, "{\"result\":\"ok\",\"match\":%i,\"low_quality\":%i,\"n_overflow\":%i}",
      n_match, n_loq, n_ovf);
  if (k<0) { return -1; }
  duk_push_string(ctx, tbuf);
  free(tbuf);
  */


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


