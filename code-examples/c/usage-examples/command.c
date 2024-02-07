#include <mongoc.h>
#include <stdio.h>
#include <bson.h>

int main(int argc, char *argv[]) {
    mongoc_client_t *client;
    mongoc_database_t *database;
    bson_error_t error;
    bson_t *command, reply;
    bson_iter_t iter;
    bson_t *doc;
    char *str;

    mongoc_init ();

    client = mongoc_client_new ("<connection string uri>");
    database = mongoc_client_get_database (client, "sample_mflix");

    command = BCON_NEW ("dbStats", BCON_INT32 (1));
    if (mongoc_database_command_simple (database, command, NULL, &reply, &error)) {
        str = bson_as_canonical_extended_json (&reply, NULL);
        printf ("%s\n", str);
        bson_free (str);
    } else {
        fprintf (stderr, "%s\n", error.message);
    }

    bson_destroy (command);
    bson_destroy (&reply);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);

    mongoc_cleanup ();

    return 0;
}
