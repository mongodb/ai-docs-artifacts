#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  mongoc_client_t *client;
  mongoc_collection_t *collection;
  mongoc_cursor_t *cursor;
  const bson_t *doc;
  bson_t query;
  bson_t options;
  bson_error_t error;
  char *str;

  mongoc_init();

  client = mongoc_client_new("<connection string uri>");
  
  if (!client) {
    fprintf(stderr, "Failed to parse URI.\n");
    return EXIT_FAILURE;
  }

  collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

  bson_init(&query);
  bson_append_utf8(&query, "directors", -1, "Barbra Streisand", -1);

  /* Pass an empty document as the options argument to mongoc_cursor_new. */
  bson_init(&options);

  cursor = mongoc_collection_find_with_opts(collection, &query, &options, NULL);
  
  while (mongoc_cursor_next(cursor, &doc)) {
    str = bson_as_canonical_extended_json(doc, NULL);
    printf("%s\n", str);
    bson_free(str);
  }

  if (mongoc_cursor_error(cursor, &error)) {
    fprintf(stderr, "Cursor Failure: %s\n", error.message); 
    return EXIT_FAILURE;
  }

  bson_destroy(&query);
  bson_destroy(&options);
  mongoc_cursor_destroy(cursor);
  mongoc_collection_destroy(collection);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  return EXIT_SUCCESS;

}
