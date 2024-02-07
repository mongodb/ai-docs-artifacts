#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_t *query;
   int64_t delete_count;
   char *uri = "<connection string uri>";

   mongoc_init ();

   client = mongoc_client_new (uri);
   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");

   query = BCON_NEW ("title", BCON_UTF8 ("Annie Hall"));

   if (mongoc_collection_delete_one (collection, query, NULL, NULL, &error)) {
      delete_count = mongoc_collection_count_documents (collection, query, NULL, 0, NULL, &error);

      if (delete_count == 1) {
         printf ("Successfully deleted one document.\n");
      } else {
         printf ("No documents matched the query. Deleted 0 documents.\n");
      }
   } else {
      fprintf (stderr, "%s\n", error.message);
   }

   bson_destroy (query);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return 0;
}
