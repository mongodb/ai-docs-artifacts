#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

#define AWS_ACCESS_KEY_ID ""
#define AWS_SECRET_ACCESS_KEY ""
#define MONGODB_CLUSTER_URL ""

int main (int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_uri_t *uri;
    bson_error_t error;
    char *str;
    bool r;

    mongoc_init ();

    str = bson_strdup_printf (
        "mongodb+srv://%s:%s@%s/?authSource=%%24external&authMechanism=MONGODB-AWS",
        AWS_ACCESS_KEY_ID, AWS_SECRET_ACCESS_KEY, MONGODB_CLUSTER_URL);

    uri = mongoc_uri_new_with_error (str, &error);
    if (!uri) {
        fprintf (stderr,
                 "failed to parse URI: %s\n"
                 "error message:       %s\n",
                 str,
                 error.message);
        bson_free (str);
        return EXIT_FAILURE;
    }

    client = mongoc_client_new_from_uri (uri);
    if (!client) {
        return EXIT_FAILURE;
    }

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     * This is where the MongoDB operation calls go.
     *
     * For this introductory example we will just ping the server.
     * * * */
    bson_t *command;
    bson_t reply;

    command = BCON_NEW ("ping", BCON_INT32 (1));
    r = mongoc_client_command_simple (
            client, "admin", command, NULL, &reply, &error);
    if (!r) {
        fprintf (stderr, "%s\n", error.message);
        return EXIT_FAILURE;
    }

    bson_destroy (command);
    bson_destroy (&reply);

    mongoc_uri_destroy (uri);
    mongoc_client_destroy (client);
    mongoc_cleanup ();

    return EXIT_SUCCESS;
}
