#ifndef MUDUK_H
#define MUDUK_H

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include <vector>
#include <string>

#include "duktape.h"

#include "cgb.hpp"

extern pthread_mutex_t conn_mutex;

typedef struct connection_info_struct
{
  int connectiontype;
  std::vector<char> str;
  char *answerstring;
  struct MHD_PostProcessor *postprocessor;
  int finished;
} con_info_t;

typedef struct global_context_t {
  std::vector<int> tid;
  std::vector<duk_context *> duk_ctx;

  std::vector<cgf_t *> cgf;
  std::vector<std::string> cgf_name;
} glob_ctx_t;

extern glob_ctx_t glob_ctx;

duk_ret_t duk_func(duk_context *ctx);

void muduk_set_timeout(time_t T);
void muduk_set_max_mem(size_t M);

void muduk_start_timeout(void);
void muduk_clear_timeout(void);
duk_bool_t muduk_timeout_check(void *udata);
int muduk_init_context(duk_context **duk_ctx);

duk_ret_t muduk_native_debug(duk_context *ctx);
duk_ret_t muduk_native_info(duk_context *ctx);
duk_ret_t muduk_native_z(duk_context *ctx);
duk_ret_t muduk_native_exit(duk_context *ctx);

duk_ret_t muduk_native_tile_pair_concordance(duk_context *ctx);

int muduk_init(duk_context **duk_ctx);
int muduk_cgf_init(glob_ctx_t *ctx);

int muduk_process(duk_context *duk_ctx, const char *data, int data_len, char **resp_str);

#endif
