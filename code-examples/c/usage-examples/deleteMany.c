#include <mongoc.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *command, reply;
    bson_t *doc = NULL;
    bson_t *regex = NULL;
    mongoc_cursor_t *cursor;
    int64_t count;

    mongoc_init();

    client = mongoc_client_new("<connection string uri>");
    collection = mongoc_client_get_collection(client, "sample_mflix", "movies");

    doc = bson_new();
    regex = BCON_NEW("$regex", "Santa");
    BSON_APPEND_DOCUMENT(doc, "title", regex);

    if (!mongoc_collection_delete_many(collection, doc, NULL, &reply, &error)) {
        fprintf(stderr, "%s\n", error.message);
        return EXIT_FAILURE;
    }

    count = bson_count_keys(&reply);
    printf("Deleted %" PRId64 " documents.\n", count);

    bson_destroy(doc);
    bson_destroy(regex);
    bson_destroy(&reply);

    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);

    mongoc_cleanup();

    return EXIT_SUCCESS;
}
