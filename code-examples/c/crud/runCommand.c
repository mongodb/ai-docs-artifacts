#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_database_t *database;
   mongoc_cursor_t *cursor;
   const bson_t *doc;
   bson_error_t error;
   bson_t *command;
   bson_t reply;
   bool retval;

   mongoc_init ();

   client = mongoc_client_new ("<connection string uri>");

   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }

   database = mongoc_client_get_database (client, "testDB");

   command = BCON_NEW ("checkMetadataConsistency", BCON_INT32 (1));

   cursor = mongoc_database_command (database,
                                      MONGOC_QUERY_NONE,
                                      0, 0, 0,
                                      command,
                                      NULL, NULL);

   while (mongoc_cursor_next (cursor, &doc)) {
      char *str;

      str = bson_as_json (doc, NULL);
      printf ("%s\n", str);
      bson_free (str);
   }

   if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "Cursor Failure: %s\n", error.message);
      return EXIT_FAILURE;
   }

   bson_destroy (command);
   mongoc_cursor_destroy (cursor);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
