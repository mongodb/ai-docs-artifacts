#include <mongoc/mongoc.h>
#include <bson/bson.h>

int main (int argc, char *argv[])
{
  mongoc_client_t *client;
  mongoc_init ();

  client = mongoc_client_new ("mongodb://localhost:27017/test?maxPoolSize=5000");
  if (!client) {
    fprintf (stderr, "Failed to parse URI.\n");
    return EXIT_FAILURE;
  }

  bson_t *command, reply;
  bson_error_t error;
  bool retval;

  command = BCON_NEW ("ping", BCON_INT32 (1));

  retval = mongoc_client_command_simple (client, "admin", command, NULL, &reply, &error);

  if (!retval) {
    fprintf (stderr, "Command failed: %s\n", error.message);
    return EXIT_FAILURE;
  }

  bson_destroy (&reply);
  bson_destroy (command);
  mongoc_client_destroy (client);

  mongoc_cleanup ();

  return EXIT_SUCCESS;
}
