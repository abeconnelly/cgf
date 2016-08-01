#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#include <vector>
#include <string>

#include "duktape.h"

#include "muduk.h"

//-------------
// custom ALLOC
//-------------

typedef struct {
  /* The double value in the union is there to ensure alignment is
   * good for IEEE doubles too.  In many 32-bit environments 4 bytes
   * would be sufficiently aligned and the double value is unnecessary.
   */
  union {
    size_t sz;
    double d;
  } u;
} alloc_hdr;

static size_t total_allocated = 0;

//static size_t MAX_ALLOCATED = 256 * 1024;  /* 256kB sandbox */
static size_t MAX_ALLOCATED = 40 * 1024 * 1024;  /* ~ 40 MiB sandbox */

void muduk_set_max_mem(size_t M) { MAX_ALLOCATED = M; }

#define MUDUK_VERBOSE_DUMP_MEMSTATE 0

static void muduk_dump_memstate(void) {
  if (MUDUK_VERBOSE_DUMP_MEMSTATE) {
    fprintf(stderr, "Total allocated: %ld\n", (long) total_allocated);
    fflush(stderr);
  }
}

static void *muduk_alloc(void *udata, duk_size_t size) {
  alloc_hdr *hdr;

  (void) udata;  /* Suppress warning. */

  if (size == 0) { return NULL; }

  if (total_allocated + size > MAX_ALLOCATED) {
    fprintf(stderr, "Sandbox maximum allocation size reached, %ld requested in muduk_alloc\n",
            (long) size);
    fflush(stderr);
    return NULL;
  }

  hdr = (alloc_hdr *) malloc(size + sizeof(alloc_hdr));
  if (!hdr) { return NULL; }
  hdr->u.sz = size;
  total_allocated += size;
  muduk_dump_memstate();
  return (void *) (hdr + 1);
}

static void *muduk_realloc(void *udata, void *ptr, duk_size_t size) {
  alloc_hdr *hdr;
  size_t old_size;
  void *t;

  (void) udata;  /* Suppress warning. */

  /* Handle the ptr-NULL vs. size-zero cases explicitly to minimize
   * platform assumptions.  You can get away with much less in specific
   * well-behaving environments.
   */

  if (ptr) {
    hdr = (alloc_hdr *) (((char *) ptr) - sizeof(alloc_hdr));
    old_size = hdr->u.sz;

    if (size == 0) {
      total_allocated -= old_size;
      free((void *) hdr);
      muduk_dump_memstate();
      return NULL;
    } else {
      if (total_allocated - old_size + size > MAX_ALLOCATED) {
        fprintf(stderr, "Sandbox maximum allocation size reached, %ld requested in muduk_realloc\n",
                (long) size);
        fflush(stderr);
        return NULL;
      }

      t = realloc((void *) hdr, size + sizeof(alloc_hdr));
      if (!t) {
        return NULL;
      }
      hdr = (alloc_hdr *) t;
      total_allocated -= old_size;
      total_allocated += size;
      hdr->u.sz = size;
      muduk_dump_memstate();
      return (void *) (hdr + 1);
    }
  } else {
    if (size == 0) {
      return NULL;
    } else {
      if (total_allocated + size > MAX_ALLOCATED) {
        fprintf(stderr, "Sandbox maximum allocation size reached, %ld requested in muduk_realloc\n",
                (long) size);
        fflush(stderr);
        return NULL;
      }

      hdr = (alloc_hdr *) malloc(size + sizeof(alloc_hdr));
      if (!hdr) {
        return NULL;
      }
      hdr->u.sz = size;
      total_allocated += size;
      muduk_dump_memstate();
      return (void *) (hdr + 1);
    }
  }
}

static void muduk_free(void *udata, void *ptr) {
  alloc_hdr *hdr;

  (void) udata;  /* Suppress warning. */

  if (!ptr) { return; }
  hdr = (alloc_hdr *) (((char *) ptr) - sizeof(alloc_hdr));
  total_allocated -= hdr->u.sz;
  free((void *) hdr);
  muduk_dump_memstate();
}

//static void muduk_fatal(void *udata, const char *msg) {
//static void muduk_fatal(duk_hthread *udata, const char *msg) {
static void muduk_fatal(duk_hthread *udata, int k, const char *msg) {
  (void) udata;  /* Suppress warning. */
  fprintf(stderr, "FATAL: %s\n", (msg ? msg : "no message"));
  fflush(stderr);
  exit(1);  /* must not return */
}

//-------------
// custom ALLOC
//-------------


//----------------------
// TIMEOUT functionality
//----------------------

static time_t curr_pcall_start = 0;
static long exec_timeout_check_counter = 0;

static time_t TIMEOUT = 500000;

void muduk_set_timeout(time_t T) { TIMEOUT = T; }
void muduk_start_timeout(void) { curr_pcall_start = time(NULL); }
void muduk_clear_timeout(void) { curr_pcall_start = 0; }
duk_bool_t muduk_timeout_check(void *udata) {
  time_t now = time(NULL);
  time_t diff = now - curr_pcall_start;

  (void) udata;

  exec_timeout_check_counter++;
  if (curr_pcall_start == 0) { return 0; }
  if (diff > TIMEOUT) { return 1; }

  return 0;
}

//----------------------
// TIMEOUT functionality
//----------------------

//--------------
// duktape setup
//--------------

int muduk_init_context(duk_context **duk_ctx) {
  duk_context *ctx;

  *duk_ctx=NULL;

  //ctx = duk_create_heap_default();
  ctx = duk_create_heap(muduk_alloc,
                        muduk_realloc,
                        muduk_free,
                        NULL,
                        muduk_fatal);

  // Load local script
  //
  if (duk_peval_file(ctx, "js/muduk.js") != 0) {
    printf("error: %s\n", duk_safe_to_string(ctx, -1));
    goto muduk_init_fail;
  }

  // register native functions?
  //
  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_info, 0);
  duk_put_prop_string(ctx, -2, "muduk_info");

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_z, 1);
  duk_put_prop_string(ctx, -2, "muduk_z");

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_exit, 0);
  duk_put_prop_string(ctx, -2, "muduk_exit");

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_debug, 1);
  duk_put_prop_string(ctx, -2, "muduk_debug");

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_tile_pair_concordance, 6);
  duk_put_prop_string(ctx, -2, "muduk_pair_conc");

  duk_push_global_object(ctx);
  duk_push_c_function(ctx, muduk_native_tile_band, 4);
  duk_put_prop_string(ctx, -2, "muduk_tile_band");

  *duk_ctx = ctx;

  return 0;
muduk_init_fail:

  printf("???\n");

  duk_destroy_heap(ctx);
  return -1;
}

//--------------
// duktape setup
//--------------

int muduk_cgf_init(glob_ctx_t *ctx) {
  int i;
  cgf_t *cgf;
  int local_verbose=1;

  std::vector<std::string> cgf_fn;

  for (i=0; i<ctx->cgf_name.size(); i++) {
    cgf_fn.push_back( ctx->data_dir + "/" + ctx->cgf_locator[i] );
  }


  if (local_verbose) { printf("loading cgf...\n"); }

  for (i=0; i<cgf_fn.size(); i++) {
    cgf = load_cgf_fn(cgf_fn[i].c_str());

    if (!cgf) {
      if (local_verbose) { printf("failed to load %s\n", cgf_fn[i].c_str()); }
      return -1;
    }
    else if (local_verbose) {
      printf("  %s loaded\n", cgf_fn[i].c_str());
    }

    ctx->cgf.push_back(cgf);
  }

  if (local_verbose) { printf("cgf loaded\n"); }

  return 0;
}
