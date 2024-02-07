/* Include necessary MongoDB C libraries */
#include <mongoc.h>

/* Function declaration for event handler*/
static void
event_handler(const mongoc_apm_command_succeeded_t *event);

int
main (int argc, char *argv[])
{
  mongoc_apm_callbacks_t *callbacks;
  mongoc_client_t *client;
  mongoc_uri_t *uri;

  mongoc_init ();

  // Replace the following with your MongoDB deployment's connection string
  uri = mongoc_uri_new ("mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority");

  client = mongoc_client_new_from_uri (uri);

  // Add the event handler to monitor a specified command
  callbacks = mongoc_apm_callbacks_new ();
  // Replace <event name> with the name of the event you are subscribing to
  mongoc_apm_set_command_succeeded_cb (callbacks, event_handler);

  mongoc_client_set_apm_callbacks (client, callbacks, NULL);

  // Ping the "admin" database to verify connection
  bson_t *command;
  bson_t reply;
  bson_error_t error;
  command = BCON_NEW ("ping", BCON_INT32 (1));
  if (mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error)) {
    printf ("Connected successfully\n");
  } else {
    fprintf (stderr, "%s\n", error.message);
  }

  mongoc_client_destroy (client);
  mongoc_uri_destroy (uri);
  mongoc_cleanup ();

  return EXIT_SUCCESS;
}

// Event handler function
static void
event_handler(const mongoc_apm_command_succeeded_t *event)
{
  char *event_json;
  // Log event data when it's emitted
  event_json = bson_as_canonical_extended_json (
      mongoc_apm_command_succeeded_get_reply (event), NULL);
  printf("Received event: %s\n", event_json);
  bson_free (event_json);
}
