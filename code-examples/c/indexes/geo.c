#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_t *keys;
   char *uri;

   mongoc_init ();
   
   uri = "mongodb+srv://<user>:<password>@<cluster-url>/?writeConcern=majority";
   client = mongoc_client_new (uri);
   
   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }
   
   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");
   
   keys = bson_new ();
   BSON_APPEND_INT32 (keys, "location.geo", 2);
   
   if (!mongoc_collection_create_index (collection, keys, NULL, &error)) {
      fprintf (stderr, "Create Index Failed: %s\n", error.message);
      return EXIT_FAILURE;
   } else {
      printf ("Index created: 2dsphere\n");
   }

   bson_destroy (keys);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);
   
   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
