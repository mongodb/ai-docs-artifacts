/* 
   Initialize a session. Perform operations within the session.
*/

mongoc_session_opt_t *session_opt =
   mongoc_session_opts_new ();
   
mongoc_client_t *client;
mongoc_collection_t *collection;
mongoc_client_session_t *session;
bson_t *filter;
bson_t *update;
bson_t *opts;
bson_error_t error;
mongoc_find_and_modify_opts_t *fam_opts;
mongoc_write_concern_t *write_concern;
mongoc_read_prefs_t *read_prefs;
char *str;
mongoc_cursor_t *cursor;

client = mongoc_client_new ("mongodb://localhost:27017");

session = mongoc_client_start_session (client, session_opt, &error);

collection = mongoc_client_get_collection (client, "myColl", "myColl");

filter = bson_new ();
BSON_APPEND_UTF8 (filter, "first_name",  "{ $lt: \"Gunter\" }");

update = BCON_NEW ("$set", "{",
                      "verified", BCON_BOOL (true),
                   "}");

opts = bson_new();
BSON_APPEND_DOCUMENT (opts, "collation",
                  BCON_NEW ("locale", "de@collation=phonebook"));

mongoc_collection_find_with_opts (collection, filter, opts, session);

cursor = mongoc_client_session_command_with_opts (client,
                                                 "admin",
                                                 filter,
                                                 read_prefs,
                                                 opts,
                                                 session,
                                                 &error);

mongoc_cursor_iterator_t iter;
mongoc_cursor_t *cursor_ptr = mongoc_cursor_iterator(&iter, cursor);
mongodb_update(collection, cursor_ptr, update, session);

mongoc_write_concern_destroy (write_concern);
mongoc_collection_destroy (collection);
mongoc_read_prefs_destroy (read_prefs);
bson_destroy (filter);
bson_destroy (update);
bson_destroy (opts);
mongoc_client_session_destroy (session);
mongoc_session_opts_destroy (session_opt);
bson_free (str);
mongoc_client_destroy (client);
