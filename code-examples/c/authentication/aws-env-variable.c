#include <mongoc/mongoc.h>
#include <stdio.h>

// Remember to specify your AWS credentials in environment variables.
const char *clusterUrl = "<MongoDB deployment url>";
const char *authMechanism = "MONGODB-AWS";

char uri [256];

int main (int argc, char *argv[]) {
  mongoc_database_t *database;
  bson_error_t error;
  bson_t *command, reply;
  bool retval;

  mongoc_init();

  snprintf(uri, sizeof uri, "mongodb+srv://%s/?authSource=%%24external&authMechanism=%s", clusterUrl, authMechanism);

  // Create a new MongoClient.
  mongoc_client_t *client = mongoc_client_new(uri);

  if (!client) {
    fprintf(stderr, "Failed to parse URI.\n");
    return EXIT_FAILURE;
  }

  database = mongoc_client_get_database(client, "admin");

  command = BCON_NEW("ping", BCON_INT32(1));
  retval = mongoc_database_command_simple(database, command, NULL, &reply, &error);

  if (!retval) {
    fprintf(stderr, "%s\n", error.message);
    return EXIT_FAILURE;
  }

  printf("Connected successfully to server.\n");

  bson_destroy(command);
  mongoc_database_destroy(database);
  mongoc_client_destroy(client);

  mongoc_cleanup();

  return EXIT_SUCCESS;
}
