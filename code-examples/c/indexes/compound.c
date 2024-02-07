#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  mongoc_client_t *client;
  mongoc_collection_t *collection;
  mongoc_cursor_t *cursor;
  const bson_t *doc;
  bson_t *query;
  bson_t *sort;
  bson_t *projection;
  bson_error_t error;
  char *str;

  mongoc_init();

  client = mongoc_client_new("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

  if (!client) {
    fprintf(stderr, "Failed to parse URI.\n");
    return EXIT_FAILURE;
  }

  collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

  mongoc_collection_create_index(collection, tmp_bson("{'type': 1, 'genre': 1}"), NULL, &error);

  query = BCON_NEW("type", "movie", "genre", "Drama");
  sort = BCON_NEW("type", BCON_INT32(1), "genre", BCON_INT32(1));
  projection = BCON_NEW("_id", BCON_INT32(0), "type", BCON_INT32(1), "genre", BCON_INT32(1));

  cursor = mongoc_collection_find_with_opts(collection, query, bson_new_from_data(sort, strlen((char*)sort)), &error);
  
  while (mongoc_cursor_next(cursor, &doc)) {
    str = bson_as_json(doc, NULL);
    printf("%s\n", str);
    bson_free(str);
  }

  if (mongoc_cursor_error(cursor, &error)) {
    fprintf(stderr, "Cursor Failure: %s\n", error.message);
    return EXIT_FAILURE;
  }

  bson_destroy(query);
  bson_destroy(sort);
  bson_destroy(projection);

  mongoc_cursor_destroy(cursor);
  mongoc_collection_destroy(collection);
  mongoc_client_destroy(client);

  mongoc_cleanup();

  return EXIT_SUCCESS;
}
