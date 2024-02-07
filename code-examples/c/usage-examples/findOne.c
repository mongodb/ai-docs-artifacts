#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

// Struct for IMDB data
typedef struct {
  int rating;
  int votes;
  int id;
} IMDB;

// Struct for Movie data
typedef struct {
  char *title;
  int year;
  date_t released; // Assuming `date_t` is a custom date type.
  char *plot;
  char *type;
  IMDB imdb;
} Movie;

// Function to run query
void run() {
  // If adding error handling.
  // bson_error_t error;
  
  // Replace the uri with your MongoDB deployment's connection string.
  const char *uri_str = "<connection string uri>";

  mongoc_client_t *client;
  mongoc_collection_t *collection;
  bson_t* query;
  mongoc_cursor_t* cursor;
  const bson_t* doc;

  // Initialize the MongoDB client library.
  mongoc_init();

  client = mongoc_client_new(uri_str);
  collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

  query = BCON_NEW("title", BCON_UTF8("The Room"));
  
  // Specify the sorting options
  bson_t sort = BSON_INITIALIZER;
  BSON_APPEND_INT32(&sort, "imdb.rating", -1);

  // Projection
  bson_t fields = BSON_INITIALIZER;
  BSON_APPEND_INT32(&fields, "_id", 0);
  BSON_APPEND_INT32(&fields, "title", 1);
  BSON_APPEND_INT32(&fields, "imdb", 1);

  // Execute the query.
  cursor = mongoc_collection_find_with_opts(collection, query, &fields, &sort);
  
  // Iterate over the results and print them out.
  while (mongoc_cursor_next(cursor, &doc)) {
    char* str = bson_as_json(doc, NULL);
    printf("%s\n", str);
    bson_free(str);
  }

  // Clean up
  bson_destroy(query);
  mongoc_cursor_destroy(cursor);
  mongoc_collection_destroy(collection);
  mongoc_client_destroy(client);

  /* If added error handling.
  if (mongoc_cursor_error (cursor, &error)) {
    fprintf (stderr, "Cursor Failure: %s\n", error.message);
    exit (EXIT_FAILURE);
  }
  */

  // Release our resources.
  mongoc_cleanup();

}

int main() {
  run();
  return 0;
}
