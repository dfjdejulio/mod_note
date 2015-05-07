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
static void *create_note_dir_config(apr_pool_t *pool, char *path);
static void *merge_note_dir_config(apr_pool_t *pool, void *baseconfig, void *addconfig);
static const char *add_note(cmd_parms *cmd, void *mconfig, char *key, char *value);

static const command_rec note_command_table[] = {
    AP_INIT_TAKE2("Note", add_note, NULL, OR_ALL, "Key/value pair to set as note."),
    {NULL}
}

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

static void *create_note_dir_config(apr_pool_t *pool, char *path)
{
    /* Our per-directory config is just a list of key/value pairs. */
    return apr_table_make(pool, 1);
}

static void *merge_note_dir_config(apr_pool_t *pool, void *baseconfig, void *addconfig)
{
    /* Since it's all just tables, use the overlay function. */
    return ap_overlay_tables(pool, addconfig, baseconfig);
}

static const char *add_note(cmd_parms *cmd, void *mconfig, char *key, char *value)
{
    /* Add the key/value pair to the table. */
    apr_table_t *notes = (apr_table_t *) mconfig;
    ap_table_set(notes, key, value);
    return NULL;
}

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
