#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

// Replace the following with values for your environment.
const char* clusterUrl = "<MongoDB cluster url>";
const char* clientPEMFile = "<path to the client pem certificate file>";
const char* authMechanism = "MONGODB-X509";

// Replacing the TLS flag
const bool tls = true;

// Connection String
char* uri;

// Function to connect to the server
void run() {
  mongoc_client_t *client;
  bson_t *command, reply;
  bson_error_t error;
  char *str;
  bool retval;

  // Starting the mongo driver
  mongoc_init();

  // Constructing the URI
  uri = bson_strdup_printf(
          "mongodb+srv://%s/?authMechanism=%s&tls=%s&tlsCertificateKeyFile=%s", 
          clusterUrl, authMechanism, (tls ? "true" : "false"), clientPEMFile);

  client = mongoc_client_new(uri);

  // Initializing the ping command
  command = BCON_NEW("ping", BCON_INT32(1));

  retval = mongoc_client_command_simple(
      client, "admin", command, NULL, &reply, &error);

  str = bson_as_json(&reply, NULL);

  if (!retval) {
    fprintf(stderr, "Error with Ping: %s\n", error.message);
    return;
  } else {
    printf("Connected successfully to server\n");
  }

  bson_free(str);
  bson_destroy(command);
  mongoc_client_destroy(client);
  mongoc_cleanup();

  return;
}

int main(int argc, char *argv[]) {
  run();
}
