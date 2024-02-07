/*
    Retrieve documents that match the "year" value "1980"
    in descending order of the value of the "title" field,
    specifying a collation that uses the index.
*/

// Defining the collation options
mongoc_collation_t* collation;
collation = mongoc_collation_new();
collation->locale = strdup("en_US");

// Creating a query and options
bson_t* query = BCON_NEW("year", BCON_INT32(1980));
bson_t* opts = BCON_NEW("sort", "{", "title", BCON_INT32(-1), "}");

// Applying the collation to the options
mongoc_find_opts_set_collation(opts, collation);

// Fetching the documents
mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(myColl, query, opts, NULL);

// Getting each document in the cursor
const bson_t *doc;
while (mongoc_cursor_next(cursor, &doc)) {
    char *str = bson_as_canonical_extended_json(doc, NULL);
    printf("%s\n", str);
    bson_free(str);
}

// Releasing resources
mongoc_cursor_destroy(cursor);
bson_destroy(query);
bson_destroy(opts);
mongoc_collation_destroy(collation);
