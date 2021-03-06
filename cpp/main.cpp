#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <stdint.h>
#include <inttypes.h>

#include <vector>
#include <string>

#include "cgb.hpp"
#include "dlug.h"

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


int main(int argc, char **argv) {
  int i, j, k;
  FILE *fp=stdin;
  char *input_fn = NULL;
  char ch;
  cgf_t *cgf, *cgf_a, *cgf_b;;
  int debug_print = 0, stats_print=0;
  int json_info_print = 0;

  cgf_t **cgfa;
  int n_cgfa=3;

  int n_match, n_ovf;

  int tilepath= -1, tilestep=-1, n_tilestep=-1;
  int tilevariant_flag = 0;

  int n_loq=0, n_tot=0;
  int lvl=0;

  int single_path_concordance=-1;
  int band_flag = 0;
  int loq_flag = 0;
  int knot_flag = 0;

  std::vector<std::string> ifns;

  std::vector<int> tilepath_v;


  while ((ch=getopt(argc, argv, "hvi:DSVp:s:l:C:n:jBLk")) != -1) switch (ch) {
    case 'h':
      show_help();
      exit(0);
      break;
    case 'j':
      json_info_print = 1;
      break;

    case 'p':
      tilepath = atoi(optarg);
      tilepath_v.push_back(tilepath);
      break;
    case 's':
      tilestep = atoi(optarg);
      break;

    case 'n':
      n_tilestep = atoi(optarg);
      break;

    case 'B':
      band_flag = 1;
      break;

    case 'L':
      loq_flag = 1;
      break;
    case 'k':
      knot_flag = 1;
      break;

    case 'V':
      tilevariant_flag = 1;
      break;

    case 'l':
      lvl = atoi(optarg);
      break;

    case 'C':
      single_path_concordance = atoi(optarg);
      break;

    case 'D':
      debug_print = 1;
      break;
    case 'S':
      stats_print=1;
      break;
    case 'i':
      input_fn = strdup(optarg);
      ifns.push_back(input_fn);
      break;
    case 'v':
      break;
    default:
      break;
  }

  //if (input_fn!=NULL) {
  if (ifns.size() > 0) {
    //if (!(fp = fopen(input_fn, "r"))) {
    if (!(fp = fopen(ifns[0].c_str(), "r"))) {
      perror(ifns[0].c_str());
      show_help();
      exit(1);
    }
  } else if (isatty(fileno(stdin))) {
    show_help();
    exit(1);
  }


  //---
  //
  //cgf = load_cgf(fp);
  cgf = load_cgf_buf(fp);
  if (cgf==NULL) {
    perror("error loading cgf");
    exit(1);
  }
  if (fp!=stdin) { fclose(fp); }

  if (knot_flag) {
    if ((tilepath<0) || (tilepath >= cgf->path_count)) {
      printf("tilepath out of range (must be within [0,%i])\n", ((int)cgf->path_count) - 1);
      show_help();
      exit(1);
    }

    if (tilestep<0) { tilestep = 0; }

    if (tilestep >= cgf->step_per_path[tilepath]) {
      printf("tilestep out of range (must be within [0,%i])\n", ((int)cgf->step_per_path[tilepath]) - 1);
      show_help();
      exit(1);
    }

    std::vector<int> allele[2];
    std::vector< std::vector<int> > loq_allele[2];

    if (n_tilestep < 0) {
      n_tilestep = ((int)cgf->step_per_path[tilepath]) - tilestep;
    }

    if ( (tilestep + n_tilestep) > ((int)cgf->step_per_path[tilepath]) ) {
      n_tilestep = ((int)cgf->step_per_path[tilepath]) - tilestep;
    }

    cgf_tile_band(cgf, tilepath, tilestep, n_tilestep, allele);
    cgf_loq_tile_band(cgf, tilepath, tilestep, n_tilestep, allele, loq_allele);

    for (i=0; i<2; i++) {
      printf("[");
      for (j=0; j<allele[i].size(); j++) {
        printf(" %i", allele[i][j]);
      }
      printf("]\n");
    }

    for (i=0; i<2; i++) {
      printf("[");
      for (j=0; j<loq_allele[i].size(); j++) {

        printf("[");
        for (k=0; k<loq_allele[i][j].size(); k++) {
          printf(" %i", loq_allele[i][j][k]);

        }
        printf(" ]");

      }
      printf("]\n");
    }

    exit(0);
  }



  if (band_flag) {

    for (i=0; i<tilepath_v.size(); i++) {
      tilepath = tilepath_v[i];

      if ((tilepath<0) || (tilepath > cgf->path_count)) {
        printf("tilepath out of range (must be within [0,%i])\n", (int)cgf->path_count);
        show_help();
        exit(1);
      }

      if ((tilestep<0) || (tilestep >= cgf->step_per_path[tilepath])) {
        printf("tilestep out of range (must be within [0,%i])\n", (int)cgf->step_per_path[tilepath]);
        show_help();
        exit(1);
      }
    }

    int idx;
    int n_tilestep_actual;
    std::vector<int> allele[2];
    std::vector< std::vector<int> > loq_allele[2];


    printf("{\n");
    for (idx=0; idx<tilepath_v.size(); idx++) {
      tilepath = tilepath_v[idx];

      if (idx>0) { printf("  ,\n"); }

      n_tilestep_actual = n_tilestep;
      if ((n_tilestep<0) || ((tilestep+n_tilestep) > cgf->step_per_path[tilepath])) {
        n_tilestep_actual = cgf->step_per_path[tilepath] - tilestep;
      }


      allele[0].clear();
      allele[1].clear();
      loq_allele[0].clear();
      loq_allele[1].clear();
      cgf_tile_band(cgf, tilepath, tilestep, n_tilestep_actual, allele);

      int fold_w = 32;
      int backup_step = 0;

      if (loq_flag) {

        if (allele[0].size() > n_tilestep_actual) {
          backup_step = allele[0].size() - n_tilestep_actual;
        }

        cgf_loq_tile_band(cgf, tilepath, tilestep-backup_step, n_tilestep_actual+backup_step, allele, loq_allele);
      }

      printf("  \"%04x\":{\n", tilepath);
      printf("    \"tilepath\":%i,\n", tilepath);
      printf("    \"start_tilestep\":%i,\n", tilestep);
      printf("    \"allele\":[\n");
      for (i=0; i<2; i++) {
        printf("      [ ");
        //for (j=0; j<allele[i].size(); j++) {
        for (j=0; j<(allele[i].size()-backup_step); j++) {
          int ele = j+backup_step;
          if (j>0) { printf(", "); }
          if ((j>0) && ((j%fold_w)==0)) { printf("\n      "); }

          printf("%i", allele[i][ele]);
        }
        printf(" ]");
        if (i<(2-1)) { printf(",\n"); }
        else { printf("\n"); }
      }
      printf("    ]");

      if (!loq_flag) {
        printf("\n");
      } else if (loq_flag) {
        printf(",\n");
        printf("    \"loq_info\":[\n");

        for (i=0; i<2; i++) {
          printf("      [ ");
          //for (j=0; j<loq_allele[i].size(); j++) {
          for (j=0; j<(loq_allele[i].size()-backup_step); j++) {
            int ele = j+backup_step;
            if (j>0) { printf(", "); }
            if ((j>0) && ((j%fold_w)==0)) { printf("\n        "); }

            printf("[");
            for (k=0; k<loq_allele[i][ele].size(); k++) {
              if (k>0) { printf(","); }
              printf(" %i", loq_allele[i][ele][k]);
            }
            printf(" ]");

          }
          printf(" ]");
          if (i<(2-1)) { printf(",\n"); }
          else { printf("\n"); }
        }
        printf("    ]\n");
      }

      printf("  }\n");

    }

    printf("}\n");
    exit(0);
  }

  //cgf_b = load_cgf_fn("data/hu826751-GS03052-DNA_B01.cgf");

  if (ifns.size()>1) {
    cgf_b = load_cgf_fn(ifns[1].c_str());
    if (cgf_b==NULL) {
      perror(ifns[1].c_str());
      exit(1);
    }

  } else {
    fprintf(stderr, "provide second cgf file");
    exit(1);
  }


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

  /*
  if (!debug_print) {
    //cgf_tile_concordance_2(&n_match, &n_ovf,
    cgf_tile_concordance_2(&n_match, &n_loq,
        cgf, cgf_b,
        //1, 0, 9000);
        1, 5607, 104);
        //1, 5607, 5);
        //1, 5600, 100);
    //printf("n_match: %i, n_ovf: %i\n", n_match, n_ovf);
    printf("n_match: %i, n_loq: %i\n", n_match, n_loq);
  }
  */

  if (lvl==0) {

    k=0;
    for (i=0; i<cgf->path_count; i++) {
      cgf_tile_concordance_0(&n_match, cgf, cgf_b, i, 0, cgf->path[i].n_tile);
      k+=n_match;
    }

    printf("level: %i, canonical match: %i\n", lvl, k);
  }

  else if (lvl==1) {
    j=0;
    k=0;
    for (i=0; i<cgf->path_count; i++) {
      cgf_tile_concordance_1(&n_match, &n_loq,
          cgf, cgf_b,
          i, 0, cgf->path[i].n_tile);
      //printf(">>> matched %d (loq %d)\n", n_match, n_loq);
      k+=n_match;
      j+=n_loq;
    }

    printf("level: %i, canonical+cache match: %i, loq: %d\n", lvl, k, j);

  }

  else if (lvl==2) {

    if (single_path_concordance != -1) {

      if (tilestep<0) { tilestep=0; }
      if (n_tilestep<0) { n_tilestep = cgf->path[single_path_concordance].n_tile - tilestep; }


      cgf_tile_concordance_2(&n_match, &n_loq,
          cgf, cgf_b,
          single_path_concordance, tilestep, n_tilestep);
          //single_path_concordance, 0, cgf->path[single_path_concordance].n_tile);
      //match_tot += n_match;
      printf("#[%x] level: %i, matched %d (loq %d)\n", single_path_concordance, lvl, n_match, n_loq);
      printf("%04x %d\n", single_path_concordance, n_match);
    }

    else {

    //int pt=0x9e;
    /*
    k=0;
    j=0;
    n_match=0;
    n_loq=0;

    cgf_tile_concordance_2(&n_match, &n_loq,
        cgf, cgf_b,
        tilepath, 0, cgf->path[tilepath].n_tile);
    printf("[%x] level: %i, matched %d (loq %d)\n", tilepath, lvl, n_match, n_loq);
    k+=n_match;
    j+=n_loq;
    */

      int match_tot = 0;

      for (tilepath=0; tilepath<cgf->path_count; tilepath++) {
        cgf_tile_concordance_2(&n_match, &n_loq,
            cgf, cgf_b,
            tilepath, 0, cgf->path[tilepath].n_tile);
        match_tot += n_match;
        //printf("#[%x] level: %i, matched %d (loq %d)\n", tilepath, lvl, n_match, n_loq);
        //printf("%04x %d\n", tilepath, n_match);
      }

      printf("#match_tot: %i\n", match_tot);
    }

    /*
    j=0;
    k=0;
    for (i=0; i<cgf->path_count; i++) {
      cgf_tile_concordance_2(&n_match, &n_loq,
          cgf, cgf_b,
          i, 0, cgf->path[i].n_tile);
      printf("[%x] level: %i, matched %d (loq %d)\n", i, lvl, n_match, n_loq);
      k+=n_match;
      j+=n_loq;
    }
    */

    //printf("level: %i, match: %i, loq: %d\n", lvl, k, j);

  }

  /*
  j=0;
  k=0;
  for (i=0; i<cgf->path_count; i++) {
  //for (i=0; i<20; i++) {
    cgf_tile_concordance_2(&n_match, &n_loq,
        cgf, cgf_b,
        i, 0, cgf->path[i].n_tile);
    printf(">>> matched %d (loq %d)\n", n_match, n_loq);
    k+=n_match;
    j+=n_loq;
  }

  printf(">>>>> tot: %i (loq: %d)\n", k, j);
  */

  /*
  k=0;
  j=0;
  for (i=0; i<cgf->path_count; i++) {
    cgf_tile_concordance_1(&n_match, &n_ovf,
        cgf, cgf_b,
        i, 0, cgf->path[i].n_tile);
    printf(">>> %d\n", n_match);
    k+=n_match;
    j+=n_ovf;
  }

  printf(">>>>> x %i (ovf %d)\n", k, j);
  */



  /*
  cgfa = (cgf_t **)malloc(sizeof(cgf_t *)*n_cgfa);
  for (i=0; i<n_cgfa; i++) {
    cgfa[i] = load_cgf_fn(input_fn);
    if (cgfa[i]==NULL) { printf("nope\n"); }
  }
  printf("ok\n");
  */

  if (json_info_print) { cgf_json_info_print(cgf); }

  if (debug_print) { debug_print_cgf(cgf); }
  if (stats_print) { stats_print_cgf(cgf); }


}
