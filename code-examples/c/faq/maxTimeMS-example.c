#include <bson/bson.h>
#include <mongoc/mongoc.h>

// Initialize and configure the MongoDB client
mongoc_client_t *client;
mongoc_collection_t *collection;
bson_t *command, reply;
bson_error_t error;
char *str;

mongoc_init();

client = mongoc_client_new("mongodb://localhost:27017/");
collection = mongoc_client_get_collection(client, "your-db-name", "your-collection-name");

command = BCON_NEW("$where", "sleep(100) || true");
if (mongoc_collection_find_with_opts(collection, command, NULL, NULL)) {
    if (!mongoc_collection_command_simple(collection, command, NULL, &reply, &error)) {
        fprintf(stderr, "%s\n", error.message);
    } else {
        str = bson_as_json(&reply, NULL);
        printf("%s\n", str);
    }
}
else {
    fprintf(stderr, "Command execution failed.");
}

bson_destroy(command);
bson_destroy(&reply);
mongoc_collection_destroy(collection);
mongoc_client_destroy(client);
mongoc_cleanup();
