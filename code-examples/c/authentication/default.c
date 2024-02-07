#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
   mongoc_client_t *client;
   mongoc_database_t *database;
   bson_error_t error;
   char *uri_str = "mongodb+srv://<username>:<password>@<MongoDB cluster url>/?authMechanism=DEFAULT";
   bool retval;

   mongoc_init ();

   client = mongoc_client_new (uri_str);
   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }

   database = mongoc_client_get_database (client, "admin");

   retval = mongoc_client_command_simple (
      client, "admin", bson_new (), NULL, NULL, &error);

   if (!retval) {
      fprintf (stderr, "%s\n", error.message);
      return EXIT_FAILURE;
   }

   printf ("Connected successfully to server.\n");

   mongoc_database_destroy (database);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
