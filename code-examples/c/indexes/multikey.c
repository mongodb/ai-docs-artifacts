#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_t *command, reply, *index_bson, *query_bson, *projection_bson;
   bson_error_t error;
   char *str;
   bool retval;

   mongoc_init ();

   /* Replace the URI string with your MongoDB deployment's connection string */
   client = mongoc_client_new (
       "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority");

   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");

   /* create multikey index */
   index_bson = BCON_NEW ("cast", BCON_INT32 (1));
   retval = mongoc_collection_create_index (
       collection, index_bson, NULL, NULL, &error);
   
   if (!retval)
   {
      fprintf (stderr, "%s\n", error.message);
   }
   
   /* query for movies with Viola Davis, no ID returned */
   query_bson = BCON_NEW ("cast", "Viola Davis");
   projection_bson = BCON_NEW ("_id", BCON_INT32 (0), "cast", BCON_INT32 (1), "title", BCON_INT32 (1));

   mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (collection, query_bson, projection_bson, NULL);

   const bson_t *doc;
   while (mongoc_cursor_next (cursor, &doc)) {
      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   mongoc_cursor_destroy (cursor);
   
   if (mongoc_cursor_error (cursor, &error))
   {
      fprintf (stderr, "Cursor Failure: %s\n", error.message);
   }

   bson_destroy (index_bson);
   bson_destroy (query_bson);
   bson_destroy (projection_bson);
   
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   
   mongoc_cleanup ();
   
   return 0;
}
