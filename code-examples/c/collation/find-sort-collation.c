#include <mongoc.h>
#include <bson.h>

// Main function
int main (int argc, char **argv) {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_t *command, reply, *sort, query = BSON_INITIALIZER;
    bson_error_t error;
    char *str;
    bool retval;

    // Initializing Mongo C Driver.
    mongoc_init ();

    client = mongoc_client_new ("mongodb://localhost:27017/");
    collection = mongoc_client_get_collection (client, "mydb", "myColl");

    // Forming the query
    BSON_APPEND_UTF8 (&query, "city", "New York");
    sort = BCON_NEW ("name", BCON_INT32 (1));

    // Creating collation options
    mongoc_collation_t *collation;
    collation = (mongoc_collation_t *) bson_malloc0 (sizeof *collation);
    collation->locale = bson_strdup ("de");

    // Query execution
    cursor = mongoc_collection_find_with_opts (collection, &query, NULL, NULL);
    mongoc_cursor_set_collation (cursor, collation);

    // Sorting the cursor.
    command = BCON_NEW ("sort", "{", "name", BCON_INT32 (1), "}");
    retval = mongoc_collection_command_simple (collection, command, NULL, &reply, &error);

    if (!retval) {
        fprintf (stderr, "Failed to run command: %s\n", error.message);
        return EXIT_FAILURE;
    }

    str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);

    // Cleaning up
    bson_free (str);
    bson_destroy (command);
    bson_destroy (&reply);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
    bson_destroy (&query);
    mongoc_client_destroy (client);
    mongoc_cleanup ();

    return EXIT_SUCCESS;
}
