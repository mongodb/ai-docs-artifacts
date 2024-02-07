/* Search Index operations in MongoDB using C driver */

#include <mongoc.h>
#include <bson.h>

int main () {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;
    bson_t *create_cmd;
    bson_t *list_cmd;
    bson_t *update_cmd;
    bson_t *drop_cmd;
    bson_t search_indexes_fields;

    mongoc_init ();

    /* Replace the following with your MongoDB deployment's connection string */
    const char *uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority";
    
    client = mongoc_client_new(uri);

    collection = mongoc_client_get_collection(client, "<databaseName>", "<collectionName>");

    /* Create a Search Index */
    create_cmd = BCON_NEW("createIndexes", "<collectionName>", "indexes", "[", "{", "name", "search1", "key", "{", "$**", "1", "}", "}", "]");
    bool retval = mongoc_collection_write_command_with_opts(collection, create_cmd, NULL, NULL, &error);
    
    /* List Search Indexes */
    list_cmd = BCON_NEW("listIndexes", "1");
    mongoc_cursor_t *cursor = mongoc_collection_command(collection, MONGOC_QUERY_NONE, 0, 0, 0, list_cmd, NULL, NULL);
    
    const bson_t *doc;
    printf("Existing search indexes:\n");
    while (mongoc_cursor_next (cursor, &doc)) {
        char *str_doc = bson_as_canonical_extended_json (doc, NULL);
        printf ("%s\n", str_doc);
        bson_free (str_doc);
    }
    
    /* Update a Search Index */
    update_cmd = BCON_NEW("name", "search1", "description", "{", "type", "string", "}");
    retval = mongoc_collection_write_command_with_opts(collection, update_cmd, NULL, NULL, &error);
    
    /* Drop (Delete) a Search Index */
    drop_cmd = BCON_NEW("dropIndexes", "<collectionName>", "index", "search1");
    retval = mongoc_collection_write_command_with_opts(collection, drop_cmd, NULL, NULL, &error);

    /* Cleanup */
    bson_destroy(create_cmd);
    bson_destroy(list_cmd);
    bson_destroy(update_cmd);
    bson_destroy(drop_cmd);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    
    mongoc_cleanup();

    return 0;
}
