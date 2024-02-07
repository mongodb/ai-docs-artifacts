// Stable API - with options

// begin serverApiVersion
#include <mongoc/mongoc.h>

// Replace the placeholders in the connection string uri with your credentials
char *uri = "mongodb+srv://<user>:<password>@<cluster-url>/test?retryWrites=true&w=majority";

// Initialize the MongoDB C Driver
mongoc_init ();
mongoc_uri_t *mongodb_uri;
mongoc_client_t *client;

/* Create a client with options to specify Stable API Version 1, return
errors for commands outside of the API version, and raise exceptions
for deprecated commands. */
mongodb_uri = mongoc_uri_new_with_error (uri, &error);
client = mongoc_client_new_from_uri (mongodb_uri);

if (!client) {
  fprintf (stderr, "%s\n", error.message);
  return EXIT_FAILURE;
}

// Assign API version 1
mongoc_client_set_server_api(client,  "1",  NULL, &error);

if (!client) {
  fprintf (stderr, "%s\n", error.message);
  return EXIT_FAILURE;
}
// end serverApiVersion
