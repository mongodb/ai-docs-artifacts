#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_database_t *database;
   bson_t reply;
   bson_error_t error;
   bool retval;

   mongoc_init();

   // Replace the placeholder with your Atlas connection string
   const char *uri_string = "<connection string>";

   {
      mongoc_uri_t *uri;
      
      uri = mongoc_uri_new_with_error (uri_string, &error);
      if (!uri) {
         fprintf (stderr,
                  "failed to parse URI: %s\n"
                  "error message:       %s\n",
                  uri_string,
                  error.message);
         return EXIT_FAILURE;
      }

      client = mongoc_client_new_from_uri (uri);
      if (!client) {
         return EXIT_FAILURE;
      }
      
      // serverApiVersion is not supported in C Driver

      mongoc_uri_destroy (uri);
   }

   database = mongoc_client_get_database (client, "admin");

   retval = mongoc_client_command_simple (
      client, "admin", tmp = BCON_NEW ("ping", BCON_INT32 (1)), NULL, &reply, &error);

   if (!retval) {
      fprintf (stderr, "Ping failed: %s\n", error.message);
      return EXIT_FAILURE;
   } else {
      printf ("Pinged your deployment. You successfully connected to MongoDB!\n");
   }

   bson_destroy (&reply);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);

   mongoc_cleanup();

   return EXIT_SUCCESS;
}
