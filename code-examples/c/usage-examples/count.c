#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *command, reply, *query;
    bson_iter_t iter;
    char *str;
    int64_t count;

    mongoc_init();

    client = mongoc_client_new("<connection string uri>");

    collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

    command = BCON_NEW("count", BCON_UTF8("movies"));
    
    if (mongoc_collection_command_simple(collection, command, NULL, &reply, &error)) {
        if (bson_iter_init(&iter, &reply) && bson_iter_find(&iter, "n")) {
            printf ("Estimated number of documents in the movies collection: %lld\n", bson_iter_as_int64 (&iter));
        }
    } else {
        fprintf (stderr, "%s\n", error.message);
    }

    query = bson_new();
    BSON_APPEND_UTF8(query, "countries", "Canada");
    count = mongoc_collection_count_documents(collection, query, NULL, NULL, NULL, &error);
    if (count != -1) {
        printf ("Number of movies from Canada: %lld\n", count);
    } else {
        fprintf (stderr, "%s\n", error.message);
    }
    
    bson_destroy(query);
    bson_destroy(command);
    bson_destroy(&reply);

    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);

    mongoc_cleanup();

    return 0;
}
