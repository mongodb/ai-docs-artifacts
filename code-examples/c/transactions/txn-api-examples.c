#include <bson/bson.h>
#include <mongoc/mongoc.h>

void coreTest (mongoc_client_t *client) {
    bson_error_t error;
    bool r;

    mongoc_session_opt_t *session_opts;
    mongoc_client_session_t *session;
    mongoc_collection_t *savings_coll;
    mongoc_collection_t *checking_coll;

    bson_t reply;
    bson_t *query, *update;

    session_opts = mongoc_session_opts_new();
    session = mongoc_client_start_session(client, session_opts, &error);

    if (!session) {
        printf ("An error occurred during the transaction: %s\n", error.message);
        return;
    }

    mongoc_client_session_start_transaction (session, NULL, &error);

    savings_coll = mongoc_client_get_collection (client, "bank", "savings_accounts");
    checking_coll = mongoc_client_get_collection (client, "bank", "checking_accounts");

    query = BCON_NEW ("account_id", BCON_UTF8 ("9876"));
    update = BCON_NEW ("$inc", "{", "amount", BCON_INT32 (-100), "}");

    r = mongoc_collection_find_and_modify_with_opts (savings_coll, query, update, NULL, &reply, &error);

    if (!r) {
        printf ("An error occurred during the transaction: %s\n", error.message);
        mongoc_client_session_abort_transaction (session, &error);
    }

    bson_reinit (&reply);
    bson_destroy (query);
    bson_destroy (update);
    
    query = BCON_NEW ("account_id", BCON_UTF8 ("9876"));
    update = BCON_NEW ("$inc", "{", "amount", BCON_INT32 (100), "}");

    r = mongoc_collection_find_and_modify_with_opts (checking_coll, query, update, NULL, &reply, &error);

    if (!r) {
        printf ("An error occurred during the transaction: %s\n", error.message);
        mongoc_client_session_abort_transaction (session, &error);
    }

    mongoc_client_session_commit_transaction (session, &error);

    mongoc_collection_destroy (checking_coll);
    mongoc_collection_destroy (savings_coll);
    mongoc_client_session_destroy (session);
}

void run (const char *uri) {
    mongoc_client_t *client;
  
    mongoc_init ();
  
    client = mongoc_client_new (uri);
  
    coreTest (client);

    mongoc_client_destroy (client);
  
    mongoc_cleanup ();
}
  
int main (int argc, char *argv[]) {
    run ("<connection string>");
    return 0;
}
