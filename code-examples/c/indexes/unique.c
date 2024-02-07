#include <mongoc/mongoc.h>
#include <stdio.h>

int main () {
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_t *command, reply;
   bson_error_t error;
   bool retval;

   mongoc_init ();

   /* Replace the URI string with your MongoDB deployment's connection string. */
   client = mongoc_client_new("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");

   command = BCON_NEW ("createIndexes", "movies", "indexes", "[",
      "{", "key", "{", "theaterId", "1", "}", "name", "theaterId", "unique", BCON_BOOL (true), "}", "]");

   retval = mongoc_collection_command_simple (collection, command, NULL, &reply, &error);

   if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      return EXIT_FAILURE;
   }

   char *str = bson_as_json (&reply, NULL);
   printf ("%s\n", str);

   bson_free (str);
   bson_destroy (command);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
