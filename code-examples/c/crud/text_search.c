#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   char *str;
   bson_t *query;
   bson_t *projection;

   mongoc_init ();

   client = mongoc_client_new ("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");

   // Text search for the string "trek"
   query = BCON_NEW ("$text", "{", "$search", "trek", "}");
   projection = BCON_NEW ("_id", BCON_BOOL(false), "title", BCON_BOOL(true));

   cursor = mongoc_collection_find_with_opts(collection, query, projection, NULL);

   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "Cursor Failure: %s\n", error.message);
      return EXIT_FAILURE;
   }

   bson_destroy (query);
   bson_destroy (projection);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   return 0;
}
