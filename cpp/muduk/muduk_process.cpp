#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "duktape.h"

#include "muduk.h"

int muduk_process(duk_context *duk_ctx, const char *data, int data_len, char **resp_str) {
  int ret=0;
  //char *query = NULL;
  char *cmd = NULL;
  const char *r=NULL;
  int local_verbose = 1;

  if (resp_str) { *resp_str = NULL; }

  if (data_len==0) { return 0; }

  cmd = (char *)malloc(sizeof(char)*(data_len+1));
  cmd[data_len] = '\0';
  memcpy(cmd, data, data_len);

  //--

  muduk_start_timeout();

  if (local_verbose) { printf("\n\n==BEG==\n"); }

  duk_push_string(duk_ctx, cmd);


  if (duk_peval(duk_ctx)!=0) {
    if (local_verbose) { printf("==END(fail)==\n\n"); }

    r = duk_safe_to_string(duk_ctx, -1);

    if (local_verbose) {
      printf("failed: %s\n", r);
    }

  } else {
    if (local_verbose) { printf("==END==\n\n"); }

    r = duk_safe_to_string(duk_ctx, -1);

    if (local_verbose) {
      printf("result:\n--\n%s\n--\n", r);
    }
  }

  muduk_clear_timeout();

  //--

  if (r && resp_str) { *resp_str = strdup(r); }

  free(cmd);

  return ret;
}
