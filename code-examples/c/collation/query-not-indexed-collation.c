#include <mongoc.h>
#include <bson.h>

/* 
   Build the query for retrieve documents match the "year" value "1980"
   and a collation that does not use the collation index
   in descending order of the value of the "title" field.
*/
const bson_t *doc;
bson_t *query;
mongoc_collection_t *collection;
mongoc_cursor_t *cursor;
mongoc_find_opts_t *opts;
mongoc_read_prefs_t *read_prefs;

query = bson_new();
BSON_APPEND_INT32(query, "year", 1980);
opts = mongoc_find_opts_new();
mongoc_find_opts_set_sort(opts, BCON_NEW("title", BCON_INT32(-1)));

/* Setting collation */
bson_t *coll = bson_new();
BSON_APPEND_UTF8(coll, "locale", "en_US");
BSON_APPEND_INT32(coll, "strength", 2);
mongoc_find_opts_set_collation(opts, coll);

/* Execute the query */
cursor = mongoc_collection_find_with_opts(collection, query, opts, read_prefs);

while (mongoc_cursor_next(cursor, &doc)) {
  char *str = bson_as_json(doc, NULL);
  printf("%s\n", str);
  bson_free(str);
}

bson_destroy(query);
bson_destroy(coll);
mongoc_find_opts_destroy(opts);
mongoc_cursor_destroy(cursor);
mongoc_collection_destroy(collection);
