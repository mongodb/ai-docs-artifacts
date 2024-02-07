#include <mongoc.h>
#include <bson.h>

int
main (int argc, char *argv[])
{
  mongoc_client_t *client;
  mongoc_collection_t *collection;
  bson_error_t error;
  bson_t *document;
  bool retval;

  mongoc_init ();

  client = mongoc_client_new ("<connection string uri>");
  collection = mongoc_client_get_collection (client, "sample_mflix", "theaters");

  document = bson_new ();
  BSON_APPEND_DOCUMENT_BEGIN (document, "$set", &child1);
  BSON_APPEND_BOOL (&child1, "is_in_ohio", true);
  bson_append_document_end (document, &child1);

  bson_t query = BSON_INITIALIZER;
  BSON_APPEND_UTF8 (&query, "location.address.zipcode", "44011");

  retval = mongoc_collection_update_many (
      collection, &query, document, NULL, NULL, &error);

  if (!retval) {
    fprintf (stderr, "%s\n", error.message);
  }

  bson_destroy (document);
  bson_destroy (&query);
  mongoc_collection_destroy (collection);
  mongoc_client_destroy (client);

  mongoc_cleanup ();

  return 0;
}
