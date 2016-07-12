/* Feel free to use this example code in any way
   you see fit (Public Domain) */

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <vector>

#include <sys/syscall.h>


#include "duktape.h"

#include "cgb.hpp"

//#define PORT            8888
#define PORT            8082
#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     20
#define MAXANSWERSIZE   512

#define GET             0
#define POST            1

#include "index-html.h"

#include "muduk.h"

static unsigned int MUDUK_THREAD_POOL_SIZE = 4;
pthread_mutex_t conn_mutex;
glob_ctx_t glob_ctx;

const char *errorpage =
    "<html><body>Error</body></html>";

static int send_page (struct MHD_Connection *connection, const char *page) {
  int ret;
  struct MHD_Response *response;

  response =
    MHD_create_response_from_buffer(
        strlen(page),
        (void *) page,
        MHD_RESPMEM_PERSISTENT);
  if (!response) return MHD_NO;

  ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
  MHD_destroy_response (response);

  return ret;
}


static int iterate_post (
    void *coninfo_cls,
    enum MHD_ValueKind kind,
    const char *key,
    const char *filename,
    const char *content_type,
    const char *transfer_encoding,
    const char *data, uint64_t off,
    size_t size)
{
  con_info_t *con_info = NULL;
  con_info = (con_info_t *)coninfo_cls;

  printf("iterate_post >>> key: %s, filename: %s, content_type: %s, xfer_enc: %s\n", key, filename, content_type, transfer_encoding);
  printf("  size: %i, off %i\n", (int)size, (int)off);

  return MHD_YES;
}

static void request_completed (void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe) {
  con_info_t *con_info = (con_info_t *)(*con_cls);

  if (con_info == NULL) { return; }
  if ((con_info) && (con_info->answerstring)) {
    free(con_info->answerstring);
  }
  delete (con_info);
  *con_cls = NULL;
}

int muduk_find_conn_idx() {
  int i, ttid;
  ttid = (int)syscall(SYS_gettid);
  for (i=0; i<glob_ctx.tid.size(); i++) {
    if (glob_ctx.tid[i] == ttid) {
      return i;
    }
  }
  return -1;
}

duk_context *muduk_find_conn() {
  int i;
  int ttid=-1;
  duk_context *duk_ctx;

  // If our thread pool capacity has been reached,
  // search for it.
  //
  if (glob_ctx.tid.size()==MUDUK_THREAD_POOL_SIZE) {

    ttid = (int)syscall(SYS_gettid);
    for (i=0; i<glob_ctx.tid.size(); i++) {
      if (glob_ctx.tid[i] == ttid) {
        return glob_ctx.duk_ctx[i];
      }
    }
    return NULL;
  }

  // Otherwise createa a new entry
  //
  pthread_mutex_lock(&conn_mutex);

  if (glob_ctx.tid.size()<MUDUK_THREAD_POOL_SIZE) {
    ttid = (int)syscall(SYS_gettid);

    for (i=0; i<glob_ctx.tid.size(); i++) {
      if (ttid == glob_ctx.tid[i]) {
        duk_ctx = glob_ctx.duk_ctx[i];
        break;
      }
    }

    if (i==glob_ctx.tid.size()) {
      muduk_init_context(&duk_ctx);
      glob_ctx.tid.push_back(ttid);
      glob_ctx.duk_ctx.push_back(duk_ctx);
    }

  }

  pthread_mutex_unlock(&conn_mutex);

  return duk_ctx;
}

static int muduk_verbose = 1;

int muduk_print_data(const char *data, int n) {
  int i;
  printf("data:\n----------\n");
  for (i=0; i<n; i++) { printf("%c", data[i]); }
  printf("\n----------\n");
}



static int answer_to_connection (
    void *cls,
    struct MHD_Connection *connection,
    const char *url,
    const char *method,
    const char *version,
    const char *upload_data,
    size_t *upload_data_size,
    void **con_cls)
{

  con_info_t *con_info;
  int new_conn = ((*con_cls)==NULL);
  duk_context *local_duk_ctx=NULL;
  int conn_idx = -1;
  int ttid = -1;


  //
  ttid = (int)syscall(SYS_gettid);
  if (muduk_verbose) { printf("[thread:%i] %s %s %s\n", ttid, method, url, version); }

  local_duk_ctx = muduk_find_conn();
  if (local_duk_ctx==NULL) { return send_page(connection, errorpage); }

  conn_idx = muduk_find_conn_idx();
  if (conn_idx<0) { return send_page(connection, errorpage); }
  //

  if (strncmp(method, "GET", 4)==0) { return send_page(connection, (char *)index_html); }


  // Setup new connection
  //
  if (new_conn) {
    con_info = new con_info_t;

    // OOM
    if (con_info==NULL) { return MHD_NO; }

    con_info->finished=0;
    con_info->postprocessor = NULL;

    con_info->answerstring = NULL;
    if (strcmp(method, "POST")==0) { con_info->connectiontype = POST; }
    else { con_info->connectiontype = GET; }

    *con_cls = (void *) con_info;
    return MHD_YES;
  }

  // GET : default page
  //
  if (strcmp(method, "GET") == 0) { return send_page (connection, (char *)index_html); }

  // POST data is the JavaScript to run.
  //
  if (strcmp(method, "POST") == 0) {
    con_info_t *con_info = (con_info_t *)(*con_cls);

    if (con_info->finished) {
      if (!con_info->answerstring) { return send_page(connection, "{}"); }
      return send_page(connection, con_info->answerstring);
    }

    if (*upload_data_size != 0) {

      if (muduk_verbose) { muduk_print_data(upload_data, *upload_data_size); }

      // Process the JavaScript request, storing ther esult in answerstring.
      // We don't want information to leak to the next session, so tear down
      // the connection here.
      //

      muduk_process(local_duk_ctx, upload_data, *upload_data_size, &(con_info->answerstring));
      duk_destroy_heap(local_duk_ctx);
      muduk_init_context(&local_duk_ctx);
      glob_ctx.duk_ctx[conn_idx] = local_duk_ctx;

      con_info->finished = 1;
      *upload_data_size = 0;

      return MHD_YES;
    } else if (NULL != con_info->answerstring) { }
  }

  return send_page (connection, errorpage);
}

int main (int argc, char **argv) {
  struct MHD_Daemon *daemon;

  muduk_cgf_init(&glob_ctx);

  daemon =
    MHD_start_daemon(
        MHD_USE_SELECT_INTERNALLY,
        PORT,
        NULL, NULL,
        &answer_to_connection, NULL,
        MHD_OPTION_THREAD_POOL_SIZE, MUDUK_THREAD_POOL_SIZE,
        MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL,
        MHD_OPTION_END);

  if (daemon==NULL) {
    printf("daemon failed to start\n");
    exit(1);
  }

  getchar();
  MHD_stop_daemon (daemon);

  return 0;
}
