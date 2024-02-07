#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
#include <unistd.h>

// Replace the uri string with your MongoDB deployment's connection string
const char *uri_str = "<connection string uri>";

static void
watch_change_db (mongoc_database_t * database)
{
   mongoc_change_stream_t *stream;
   bson_error_t error;
   const bson_t *doc;

   const char * haikus = "haikus";

   stream = mongoc_database_watch (database,(bson_t *) haikus,NULL);
   if (stream) {
      printf ("received a change to the collection: \t\n");

      while (mongoc_change_stream_next (stream, &doc)) {
         char * str = bson_as_json (doc, NULL);
         printf ("%s\n", str);
         bson_free (str);
      }

      sleep (1);

      bson_t * insert = BCON_NEW (
         "title", "Record of a Shriveled Datum",
         "content", "No bytes, no problem. Just insert a document, in MongoDB"
      );
      
      bool r = mongoc_database_insert_one (database, haikus, insert, NULL, &error);
      bson_destroy (insert);
     
      if (!r) {
         fprintf (stderr, "Error inserting: %s\n", error.message);
      }

      sleep (1);
      
      if (mongoc_change_stream_error_document (stream, &error, NULL)) {
         fprintf (stderr, "Error reading change stream: %s\n", error.message);
         return;
      }
      
      mongoc_change_stream_close (stream);
      printf ("closed the change stream\n");
      mongoc_change_stream_destroy (stream);

      mongoc_uri_destroy (uri);  
      mongoc_database_destroy (database);
      mongoc_client_destroy (client);
      
      return 0;
   }
}

int
main (int argc, char *argv[])
{
   mongoc_client_t *client = mongoc_client_new (uri_str);

   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }

   mongoc_database_t * database = mongoc_client_get_database (client, "insertDB");
   watch_change_db (database);
}
