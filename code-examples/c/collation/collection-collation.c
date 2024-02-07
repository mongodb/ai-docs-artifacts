mongoc_collection_t *collection;
mongoc_database_t *database;
bson_t *command;
bson_t reply;
bson_error_t error;

/* Create the "souvenirs" collection and specify the French Canadian collation */
command = BCON_NEW ("create", BCON_UTF8 ("souvenirs"),
                    "collation", "{",
                    "locale", BCON_UTF8 ("fr_CA"),
                    "}");

collection = mongoc_database_get_collection (database, "souvenirs");

if (!mongoc_database_command_simple (database, command, NULL, &reply, &error)) {
    fprintf (stderr, "%s\n", error.message);
}

bson_destroy (command);
bson_destroy (&reply);
mongoc_collection_destroy (collection);
mongoc_database_destroy (database);
