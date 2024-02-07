#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    bson_error_t error;

    mongoc_init();

    client = mongoc_client_new(
        "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

    collection = mongoc_client_get_collection(client, "sample_mflix", "theaters");

    // Start proximity geo example
    // Find theaters near a specific location
    bson_t *query = BCON_NEW(
        "location.geo", "{",
          "$near", "{",
            "$geometry", "{",
              "type", "Point",
              "coordinates", "[", BCON_DOUBLE(-73.9667), BCON_DOUBLE(40.78), "]",
            "}",
            "$maxDistance", BCON_INT32(10000),
          "}",
        "}"
    );

    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // Check if any documents were found
    if (!mongoc_cursor_more(cursor)) {
        printf("No documents found!\n");
    }

    // Iterate through the found documents and display them
    const bson_t *doc;
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    // Clean up
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);

    // Start range geo example
    // Find theaters within a specific geographic range
    query = BCON_NEW(
        "location.geo", "{",
          "$geoWithin", "{",
            "$geometry", "{",
              "type", "Polygon",
              "coordinates", "[",
                "[", 
                  "[", BCON_DOUBLE(-72), BCON_DOUBLE(40), "]",  // Polygon coordinates defining the range
                  "[", BCON_DOUBLE(-74), BCON_DOUBLE(41), "]",
                  "[", BCON_DOUBLE(-72), BCON_DOUBLE(39), "]",
                  "[", BCON_DOUBLE(-72), BCON_DOUBLE(40), "]",
                "]",
              "}",
            "}",
          "}",
        "}"
    );

    cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);

    // Check if any documents were found
    if (!mongoc_cursor_more(cursor)) {
        printf("No documents found!\n");
    }

    // Iterate through the found documents and display them
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }

    // Clean up
    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);

    mongoc_cleanup();

    return 0;
}
