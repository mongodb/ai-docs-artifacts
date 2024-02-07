// Stable API - no options

#include <mongoc.h>

// Replace the placeholders in the connection string uri with your credentials
const char *uriString = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";

// Create a client with options to specify Stable API Version 1
mongoc_client_t *client;
mongoc_uri_t *uri;
mongoc_server_api_t *server_api;
mongoc_server_api_version_t api_version = MONGOC_SERVER_API_V1;
mongoc_server_api_t api;

mongoc_init ();
uri = mongoc_uri_new_with_error (uriString, &error);
mongoc_server_api_version_from_string ("1", &api_version);
server_api = mongoc_server_api_new (api_version);
mongoc_uri_set_server_api (uri, server_api);
client = mongoc_client_new_from_uri (uri);
mongoc_server_api_destroy (server_api);
mongoc_uri_destroy (uri);
