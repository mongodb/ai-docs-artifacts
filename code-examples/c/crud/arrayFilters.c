#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>

int
main (int argc, char *argv[])
{
   mongoc_client_t *client;
   mongoc_collection_t *collection;
   bson_error_t error;
   bson_t *update;

   mongoc_init ();

   client = mongoc_client_new ("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");
   collection = mongoc_client_get_collection (client, "test", "testColl");

   // Updating the first array element example
   bson_t *query = BCON_NEW ("entries.x", "{", "$type", BCON_UTF8 ("string"), "}");
   update = BCON_NEW ("$inc", "{", "entries.$.y", BCON_INT64 (33), "}");
   bool r = mongoc_collection_update_one (collection, query, update, NULL, NULL, &error);
   if (!r) {
      fprintf (stderr, "%s\n", error.message);
   }
   bson_destroy (query);
   bson_destroy (update);

   // Updating all array elements example
   query = BCON_NEW ("date", BCON_UTF8 ("5/15/2023"));
   update = BCON_NEW ("$unset", "{", "calls.$[].duration", BCON_UTF8 (""), "}");
   r = mongoc_collection_update_one (collection, query, update, NULL, NULL, &error);
   if (!r) {
      fprintf (stderr, "%s\n", error.message);
   }
   bson_destroy (query);
   bson_destroy (update);

   // Array filters identifier example
   query = BCON_NEW ("date", BCON_UTF8 ("11/12/2023"));
   update = BCON_NEW ("$mul", "{", "items.$[i].quantity", BCON_INT32 (2), "}");
   mongoc_update_flags_t flags = MONGOC_UPDATE_MULTI_UPDATE;
   bson_t *opts = BCON_NEW ("arrayFilters", "[", "{", "i.recipe", BCON_UTF8 ("Fried rice"), "i.item", "{", "$not", "{", "$regex", BCON_UTF8 ("oil"), "}", "}", "}", "]");
   r = mongoc_collection_update_one_with_opts (collection, query, update, opts, NULL, &error);
   if (!r) {
      fprintf (stderr, "%s\n", error.message);
   }
   bson_destroy (query);
   bson_destroy (update);
   bson_destroy (opts);

   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return EXIT_SUCCESS;
}
