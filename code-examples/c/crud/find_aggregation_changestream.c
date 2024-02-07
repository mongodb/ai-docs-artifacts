#include <bson.h>
#include <mongoc.h>
#include <unistd.h>

int main() {

    mongoc_client_t *client;
    mongoc_collection_t *collection;
    mongoc_cursor_t *cursor;
    bson_error_t error;
    bson_t *update = NULL;
    bson_t *query = NULL;
    bson_t *doc = NULL;
    bson_t *pipeline;
    const bson_t *doc_returned;
    const char *uristr = "mongodb://<user>:<password>@<cluster-url>?w=majority";
	  const char *dbname = "test";
    const char *collname = "orders";

    mongoc_init();

    client = mongoc_client_new(uristr);
    collection = mongoc_client_get_collection(client, dbname, collname);

    // start find crud example
    // create and populate query
    query = bson_new();
    BSON_APPEND_UTF8(query, "name", "Lemony Snicket");
    
    // query the collection
    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    bson_destroy(query);
    // end find crud example

    // start aggregate crud example
    pipeline = BCON_NEW ("pipeline", "[", "{", "$match", "{", "date", "{", "$gte", BCON_DATE_TIME(time(0)*1000 - 1000*3600*24*7), "$lt", BCON_DATE_TIME(time(0)*1000), "}", "}", "}", "{", "$group", "{", "_id", "$status", "count", "{", "$sum", BCON_INT32(1), "}", "}", "}","]");
    cursor = mongoc_collection_aggregate(collection, MONGOC_QUERY_NONE, pipeline, NULL, NULL);
    bson_destroy(pipeline);
    // end aggregate crud example

    // Cannot implement change streaming CRUD operation in MongoDB C Driver

    // start insert crud example
    // create and populate new document 
    doc = bson_new();
    bson_oid_init(&oid, NULL);
    BSON_APPEND_OID(doc, "_id", &oid);
    BSON_APPEND_DATE_TIME(doc, "date", time(0)*1000);
    BSON_APPEND_UTF8(doc, "address", "667 Dark Avenue, San Francisco, CA, 94110");
    BSON_APPEND_UTF8(doc, "name", "Lemony Snicket");
    BSON_APPEND_DOCUMENT_BEGIN(doc, "items", &item_doc);
    BSON_APPEND_UTF8(&item_doc, "name", "pizza");
    BSON_APPEND_UTF8(&item_doc, "toppings", "pepperoni, mushrooms");
    BSON_APPEND_UTF8(&item_doc, "notes", "extra shrooms");
    BSON_APPEND_INT64(&item_doc, "price", 11);
    BSON_APPEND_UTF8(doc, "status", "created");
    bson_append_document_end(doc, &item_doc);

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
    }
    bson_destroy(doc);
    // end insert crud example

    // start update crud example
    // create and populate new document 
    doc = BCON_NEW("address", "667 Dark Avenue, San Francisco, CA, 94110");
    update = BCON_NEW("$set", "{", "address", "667 Dark Avenue, San Francisco, CA, 94103", "}");

    if (!mongoc_collection_update_one(collection, doc, update, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
    }
    bson_destroy(doc);
    bson_destroy(update);
    // end update crud example

    // start delete crud example
    // create and populate new document 
    doc = BCON_NEW("address", "13 Lousy Lane", "name", "Violet Baudelaire");
    
    if (!mongoc_collection_delete_one(collection, doc, NULL, NULL, &error)) {
        fprintf(stderr, "%s\n", error.message);
    }
    bson_destroy(doc);
    // end delete crud example
	
    mongoc_cursor_destroy(cursor);
    bson_destroy(result);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);

    mongoc_cleanup();

    return 0;
}
