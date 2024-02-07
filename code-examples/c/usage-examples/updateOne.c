#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
   mongoc_client_t      *client;
   mongoc_collection_t  *collection;
   mongoc_write_concern_t *write_concern;
   bson_error_t         error;
   bson_t               *update = NULL;
   bson_t               *query = NULL;
   char                 *str;
   int                   exitcode = EXIT_FAILURE;

   mongoc_init();

   client = mongoc_client_new ("<connection string uri>");
   collection = mongoc_client_get_collection (client, "sample_mflix", "movies");
   write_concern = mongoc_write_concern_new ();

   mongoc_write_concern_set_wmajority (write_concern, 10000 /* 10 seconds */);
   mongoc_collection_set_write_concern (collection, write_concern);
   
   query = BCON_NEW ("title", BCON_UTF8 ("Random Harvest"));
   update = BCON_NEW ("$set", "{",
                        "plot", BCON_UTF8 ("A harvest of random numbers, such as: X"),
                     "}");

   // setting upsert as true
   mongoc_update_flags_t upsert_flag = MONGOC_UPDATE_UPSERT;
   if (mongoc_collection_update (collection, upsert_flag, query, update, write_concern, &error)) {
      exitcode = EXIT_SUCCESS;
   }

   // Log the number of matching and modified documents
   printf("Modified %d entries\n", mongoc_collection_count(collection, MONGOC_QUERY_NONE, NULL, 0, 0, NULL, NULL));

   if (query)
      bson_destroy (query);

   if (update)
      bson_destroy (update);
   
   mongoc_write_concern_destroy (write_concern);
   mongoc_collection_destroy (collection);
   mongoc_client_destroy (client);

   mongoc_cleanup ();

   return exitcode;
}
