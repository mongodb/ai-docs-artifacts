#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
  mongoc_client_t *client;
  mongoc_database_t *database;
  bson_error_t error;
  char *user = "<username>";
  char *password = "<password>";
  char *db_url = "<MongoDB cluster url>";

  mongoc_init ();

  // Encode the user information
  char *str = bson_strdup_printf (
     "mongodb+srv://%s:%s@%s/?authMechanism=%s",
      user, password, db_url, "SCRAM-SHA-256"
     );

  client = mongoc_client_new (str);
  
  if (!client) {
    fprintf (stderr, "Failed to parse URI.\n");
    return EXIT_FAILURE;
  }

  database = mongoc_client_get_database (client, "admin");

  // Test the connection
  if (mongoc_client_command_simple (client, "admin", tmp = BCON_NEW ("ping", BCON_INT32 (1)), NULL, &error)) {
    printf ("Connected successfully to server\n");
  } else {
    fprintf (stderr,
             "Ping failed (%s): %s\n",
             error.domain == MONGOC_ERROR_SERVER_SELECTION
             ? "server selection error" : "other error",
             error.message);
    
    return EXIT_FAILURE;
  }

  // Close connection
  mongoc_database_destroy (database);
  mongoc_client_destroy (client);
  bson_free (str);

  mongoc_cleanup ();
  
  return EXIT_SUCCESS;
}
