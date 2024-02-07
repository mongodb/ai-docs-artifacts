#include <bcon.h>
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_oid_t oid;
    bson_t *doc;
    char *str;
    bool retval;

    mongoc_init ();

    client = mongoc_client_new ("<connection string uri>");
    collection = mongoc_client_get_collection (client, "insertDB", "foods");

    bson_t *foods[] = {
            BCON_NEW ("name", "cake", "healthy", BCON_BOOL(false)),
            BCON_NEW ("name", "lettuce", "healthy", BCON_BOOL(true)),
            BCON_NEW ("name", "donut", "healthy", BCON_BOOL(false)),
    };

    size_t n = sizeof(foods) / sizeof(foods[0]);
    size_t insertedCount = 0;
    for (size_t i=0; i<n; i++) {
        if (!(retval = mongoc_collection_insert_one (collection, foods[i], NULL, NULL, &error))) {
            fprintf (stderr, "%s\n", error.message);
            return EXIT_FAILURE;
        }
        insertedCount++;
        bson_destroy (foods[i]);
    }

    printf ("%lu documents were inserted\n", insertedCount);

    mongoc_collection_destroy (collection);
    mongoc_client_destroy (client);

    mongoc_cleanup ();

    return EXIT_SUCCESS;
}
