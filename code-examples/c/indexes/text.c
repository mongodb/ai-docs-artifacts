#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[]) {
    mongoc_client_t *client;
    mongoc_database_t *database;
    mongoc_collection_t *collection;
    bson_t *command, reply, *index;
    bson_error_t error;
    char *str;
    bool retval;

    mongoc_init ();

    client = mongoc_client_new ("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
    database = mongoc_client_get_database (client, "testDB");
    collection = mongoc_client_get_collection (client, "testDB", "blogPosts");

    /* Create a text index on the "title" and "body" fields */
    index = BCON_NEW ("title", BCON_UTF8 ("text"), "body", BCON_UTF8 ("text"));
    
    retval = mongoc_collection_create_index (
      collection, index, NULL, &reply, &error);

    if (!retval) {
        fprintf (stderr, "%s\n", error.message);
        return EXIT_FAILURE;
    }
    
    str = bson_as_json (&reply, NULL);
    printf ("%s\n", str);

    /* Query for documents where body or title contain "life ahead" */
    bson_t *query = BCON_NEW ("$text", "{", "$search", "life ahead", "}");

    /* Show only the title field */
    bson_t *projection = BCON_NEW ("_id", BCON_BOOL (false), "title", BCON_BOOL (true));

    /* Execute the find operation */
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts (
      collection, query, projection, NULL);

    const bson_t *loop_doc;
    while (mongoc_cursor_next (cursor, &loop_doc)) {
        char *string = bson_as_json (loop_doc, NULL);
        printf ("%s\n", string);
        bson_free (string);
    }

    bson_destroy (index);
    bson_destroy (query);
    bson_destroy (projection);
    bson_free (str);
    mongoc_cursor_destroy (cursor);
    mongoc_collection_destroy (collection);
    mongoc_database_destroy (database);
    mongoc_client_destroy (client);
    mongoc_cleanup ();

    return EXIT_SUCCESS;
}
