#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

// Replace the following with values for your environment.
char *username = "<username>";
char *password = "<password>";
char *clusterUrl = "<MongoDB cluster url>";

char *uri;

// SCRAM-SHA-1 authentication mechanism
char *authMechanism = "SCRAM-SHA-1";

int main (int argc, char *argv[]) {
    mongoc_client_t *client;
    mongoc_database_t *database;
    bson_error_t error;

    mongoc_init();

    // Create connection URL
    char *encodedUsername = mongoc_uri_encode (username);
    char *encodedPassword = mongoc_uri_encode (password);
    asprintf(&uri, "mongodb+srv://%s:%s@%s/?authMechanism=%s", 
                 encodedUsername, encodedPassword, clusterUrl, authMechanism);

    // Create a new MongoClient
    client = mongoc_client_new (uri);

    // Connecting to the server
    if (!mongoc_client_get_server_status (client, NULL, NULL, &error)) {
        fprintf (stderr, "Error with connection: %s\n", error.message);
        return EXIT_FAILURE;
    } else {
        printf ("Connected successfully to server\n");
    }

    // Ensures that the client will close when you finish/error
    mongoc_client_destroy (client);
    mongoc_cleanup();

    free(encodedUsername);
    free(encodedPassword);
    free(uri);

    return EXIT_SUCCESS;
}
