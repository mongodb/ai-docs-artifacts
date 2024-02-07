#include <bson.h>
#include <mongoc.h>
#include <stdio.h>

void
cleanUp (mongoc_collection_t *collection)
{
   bson_error_t error;

   if (!mongoc_collection_drop (collection, &error)) {
      printf ("Drop failed: %s\n", error.message);
   }
}

void
setup (mongoc_client_t *client)
{
   bson_error_t error;
   mongoc_collection_t *collection;
   bson_t issue1 = BSON_INITIALIZER;
   bson_t issue2 = BSON_INITIALIZER;
   bson_t issue3 = BSON_INITIALIZER;

   collection = mongoc_client_get_collection (client, "testdb", "customers");

   BSON_APPEND_INT32 (&issue1, "_id", 98765);
   bson_append_array_begin (&issue1, "orders", -1, &issue2);
   bson_append_array_end (&issue1, &issue2);
   if (!mongoc_collection_insert_one (collection, &issue1, NULL, NULL, &error)) {
      printf ("Insert failed: %s\n", error.message);
   }

   collection = mongoc_client_get_collection (client, "testdb", "inventory");

   BSON_APPEND_UTF8 (&issue2, "item", "sunblock");
   BSON_APPEND_INT32 (&issue2, "item_id", 5432);
   BSON_APPEND_INT32 (&issue2, "qty", 85);
   if (!mongoc_collection_insert_one (collection, &issue2, NULL, NULL, &error)) {
      printf ("Insert failed: %s\n", error.message);
   }

   BSON_APPEND_UTF8 (&issue3, "item", "beach towel");
   BSON_APPEND_INT32 (&issue3, "item_id", 7865);
   BSON_APPEND_INT32 (&issue3, "qty", 41);
   if (!mongoc_collection_insert_one (collection, &issue3, NULL, NULL, &error)) {
      printf ("Insert failed: %s\n", error.message);
   }

   bson_destroy (&issue1);
   bson_destroy (&issue2);
   bson_destroy (&issue3);

   mongoc_collection_destroy (collection);
}


void
main (void)
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   mongoc_client_session_t *session;
   bson_error_t error;
   bson_t *opts;
   bson_t reply;
   char *str;

   mongoc_init ();

   client = mongoc_client_new ("mongodb://localhost:27017/?appname=client-example");
   collection = mongoc_client_get_collection (client, "testdb", "collection");

   cleanUp (collection);
   setup (client);

   session = mongoc_client_start_session (client, NULL, &error);
   if (!session) {
      fprintf (stderr, "Error creating session: %s\n", error.message);
      return;
   }

   opts = BCON_NEW ("lsid", "{", "id", BCON_BINARY (BSON_SUBTYPE_UUID, gLsid, 16), "}");

   if (mongoc_client_session_start_transaction (session, NULL, &error) &&
       perform_actions (client, session, &error)) {
      str = bson_as_json (&reply, NULL);
      printf ("%s\n", str);
   }
  
   mongoc_client_session_abort_transaction (session, &error);
   mongoc_client_session_destroy (session);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();
}
