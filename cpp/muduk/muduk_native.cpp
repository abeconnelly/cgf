#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include <vector>
#include <string>

#include "duktape.h"

#include "muduk.h"

duk_ret_t muduk_native_debug(duk_context *ctx) {
  const char *cmd = duk_get_string(ctx, 0);

  if (cmd==NULL)  {
    printf("options:\n");
    printf("  get_top\n");
    printf("  get_top_index\n");
  }

  if (strcmp(cmd, "get_top")==0) {
    printf("(debug) duk_get_top(): %i\n", (int)duk_get_top(ctx));
  }
  else if (strcmp(cmd, "get_top_index")==0) {
    printf("(debug) duk_get_top(): %i\n", (int)duk_get_top_index(ctx));
  }
  else {
    printf("options:\n");
    printf("  get_top\n");
    printf("  get_top_index\n");
  }

  return 1;
}


duk_ret_t muduk_native_exit(duk_context *ctx) {

  printf("hard exit\n");

  exit(0);

  return 1;
}

duk_ret_t muduk_native_info(duk_context *ctx) {
  int i, k;
  char *tbuf;
  std::string resp;

  printf("muduk_native_info..\n");
  for (i=0; i<glob_ctx.tid.size(); i++) {
    printf(" ttid: %i (duk_ctx %p)\n", glob_ctx.tid[i], glob_ctx.duk_ctx[i]);
  }


  // create json string resonpose
  //
  resp += "{";
  k = asprintf(&tbuf, "\"n_thread\":%i", (int)glob_ctx.tid.size());
  if (k<0) { return -1; }

  resp += tbuf;
  free(tbuf);

  resp += ",";
  resp += "\"thread\":[";
  for (i=0; i<glob_ctx.tid.size(); i++) {
    if (i>0) { resp += ","; }
    k = asprintf(&tbuf, "%i", glob_ctx.tid[i]);
    if (k<0) { return -1; }
    resp += tbuf;
    free(tbuf);
  }
  resp += "]";

  resp += ",";
  resp += "\"response\":\"ok\"";
  resp += "}";

  //duk_push_string(ctx, "{\"result\":\"ok\"}");
  duk_push_string(ctx, resp.c_str());

  return 1;
}

duk_ret_t muduk_native_z(duk_context *ctx) {
  int i;
  int val = duk_require_int(ctx, 0);

  printf("muduk_native_z..\n");

  for (i=0; i<val; i++) {
    printf(" >> %i %i\n", i, val);
  }

  duk_push_string(ctx, "{\"result\":\"ok\"}");

  return 1;
}



