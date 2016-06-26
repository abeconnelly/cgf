#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include "duktape.h"

#include "muduk.h"

glob_ctx_t glob_ctx;

int main(int argc, char **argv) {
  char *cbuf = NULL;
  int n_cbuf = 1024;

  duk_context *ctx;

  muduk_init_context(&ctx);

  if (ctx==NULL) {
    printf(">> could not allocate duk_context, exiting\n");
    exit(1);
  }

  glob_ctx.tid.push_back(-1);
  glob_ctx.duk_ctx.push_back(ctx);

  cbuf = (char *)malloc(sizeof(char)*n_cbuf);

  while (1) {
    printf("> "); fflush(stdout);

    fgets(cbuf, n_cbuf, stdin);

    muduk_start_timeout();

    duk_push_string(ctx, cbuf);
    if (duk_peval(ctx)!=0) {
      printf("eval failed: %s\n", duk_safe_to_string(ctx, -1));
    } else {
      printf("ok: %s\n", duk_safe_to_string(ctx, -1));
    }

    muduk_clear_timeout();

  }

  return 0;
}

