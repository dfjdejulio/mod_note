#include "httpd.h"
#include "http_core.h"
#include "http_protocol.h"
#include "http_request.h"

#include "apr_strings.h"
#include "apr_network_io.h"
#include "apr_md5.h"
#include "apr_sha1.h"
#include "apr_hash.h"
#include "apr_base64.h"
#include "apr_dbd.h"
#include <apr_file_info.h>
#include <apr_file_io.h>
#include <apr_tables.h>
#include "util_script.h"

/* prototypes */
static void register_note_hooks(apr_pool_t *pool);
static int set_notes(request_rec *r);

/* apache module struct */
module AP_MODULE_DECLARE_DATA note_module =
  {
    STANDARD20_MODULE_STUFF,	
    create_note_dir_config, /* Per-directory configuration handler */
    merge_note_dir_config,  /* Merge handler for per-directory configurations */
    NULL,                   /* Per-server configuration handler */
    NULL,                   /* Merge handler for per-server configurations */
    note_command_table,        /* Any directives we may have for httpd */
    register_note_hooks     /* Our hook registering function */
  };

static void register_note_hooks(apr_pool_t *pool)
{
  ap_hook_handler(set_notes, NULL, NULL, APR_HOOK_FIRST);
}

static int set_notes(request_rec *r)
{
  /* Fetch the notes that have been configured. */
  apr_table_t *notes = ap_get_module_config(r->per_dir_config, &note_module);
  /* Overlay them onto the request's existing notes. */
  r->notes = ap_overlay_tables(r->pool, notes, r->notes);
  /* Tell Apache it has to continue processing. */
  return DECLINED;
}
