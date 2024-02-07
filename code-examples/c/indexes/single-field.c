#include <bson/bson.h>
#include <mongoc/mongoc.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_t* bipolar;
   bson_t* query;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   char *str;

   mongoc_init ();

   client = mongoc_client_new (
      "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");

   /* Creating index */
   bson_t *keys = bson_new();
   BSON_APPEND_INT32(keys, "title", 1);
   mongoc_collection_create_index(collection, keys, NULL, NULL);
   bson_destroy(keys);

   /* Query */
   query = bson_new ();
   BSON_APPEND_UTF8(query, "title", "Batman");
   cursor = mongoc_collection_find_with_opts (
      collection,
      query,
      NULL,  /* additional options */
      NULL); /* read prefs, NULL for default */

   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   bson_destroy (query);
   mongoc_cursor_destroy (cursor);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return 0;
}
