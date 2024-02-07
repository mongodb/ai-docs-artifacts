#include <mongoc.h>
#include <bson.h>
#include <stdio.h>

int main (int argc, char **argv)
{
   mongoc_client_t *client;
   bson_error_t error;
   char *str;
   bson_t command, reply;
   bson_t *opts;
   
   // Initialize the MongoDB C driver.
   mongoc_init ();

   // Replace the following with values from your environment.
   const char *mongourl = "mongodb+srv://<username>:<password>@<MongoDB cluster url>/?authMechanism=SCRAM-SHA-256&tls=true&tlsCertificateKeyFile=<path to PEM file>";

   // Create a new MongoDB client.
   client = mongoc_client_new (mongourl);
   if (!client) {
      fprintf (stderr, "Failed to parse URI.\n");
      return EXIT_FAILURE;
   }
   
   // Establish connection and verify connection
   bson_init (&command);
   bson_append_int32 (&command, "ping", 4, 1);
   
   // Run the 'ping' command to verify connection
   if (!mongoc_client_command_simple (client, "admin", &command, NULL, &reply, &error)) {
      fprintf (stderr, "Ping failed: %s\n", error.message);
   } else {
       // Print successful connection message
       printf ("Connected successfully to server\n");
   }

   str = bson_as_json (&reply, NULL);
   
   // Clean up retrieved data
   bson_free (str);
   bson_destroy (&reply);
   bson_destroy (&command);

   // Ensures that the client will close when you finish/error
   mongoc_client_destroy (client);

   // Release resources.
   mongoc_cleanup ();
   
   return EXIT_SUCCESS;
}
