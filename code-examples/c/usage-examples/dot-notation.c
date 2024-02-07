#include <bson/bson.h>
#include <mongoc/mongoc.h>

// start-no-error
mongoc_collection_t* collection;
mongoc_client_t* client;
bson_error_t error;
bson_t* doc;

client = mongoc_client_new ("<your mongodb url>");
collection = mongoc_client_get_collection (client, "<your db>", "<your collection>");

doc = BCON_NEW ("$set", "{", "field.nested", BCON_UTF8 ("A string"), "}");

if (!mongoc_collection_update_one (collection, NULL, doc, NULL, NULL, &error)) {
  printf("%s\n", error.message);
}
// end-no-error

// start-error
doc = BCON_NEW ("field", "{", "nested", BCON_UTF8 ("A string"), "}")

if (!mongoc_collection_update_one (collection, NULL, doc, NULL, NULL, &error)) {
  printf("%s\n", error.message);
}

mongoc_collection_destroy(collection);
mongoc_client_destroy(client);
// end-error

// start-no-key
client = mongoc_client_new ("<your mongodb url>");
collection = mongoc_client_get_collection (client, "<your database>", "<your collection>");
 
bson_t* query = bson_new ();
BSON_APPEND_UTF8 (query, "age", "Accepts any type!");

mongoc_cursor_t* cursor = mongoc_collection_find_with_opts (collection, query, NULL, NULL);

bson_destroy(query);
mongoc_collection_destroy(collection);
mongoc_client_destroy(client);
// end-no-key
