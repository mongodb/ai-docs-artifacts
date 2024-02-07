#include <mongoc/mongoc.h>
#include <bson/bson.h>

// Replace the uri string with your MongoDB deployment's connection string.
const char *uri_string = "<connection string uri>";

mongoc_client_t *client;
mongoc_collection_t *collection;
char *str;
bson_error_t error;
mongoc_change_stream_t *stream;
const bson_t *doc;

mongoc_init ();

client = mongoc_client_new (uri_string);
collection = mongoc_client_get_collection (client, "insertDB", "haikus");
stream = mongoc_collection_watch (
   collection, tmp_bson ("{}"), NULL); // Create a pipeline with an empty document to watch for all changes

if (mongoc_change_stream_error_document (stream, &error, &doc)) {
  fprintf (stderr, "Error: %s\n", error.message);
  return EXIT_FAILURE;
}

while (mongoc_change_stream_next (stream, &doc)) {
  str = bson_as_json (doc, NULL);
  printf ("%s\n", str);
  bson_free (str);
}

if (mongoc_change_stream_error_document (stream, &error, &doc)) {
  str = bson_as_json (doc, NULL);
  fprintf (stderr, "Error: %s, Document: %s\n", error.message, str);
  bson_free (str);
}

mongoc_change_stream_destroy (stream);
mongoc_collection_destroy (collection);
mongoc_client_destroy (client);
mongoc_cleanup ();

return EXIT_SUCCESS;
