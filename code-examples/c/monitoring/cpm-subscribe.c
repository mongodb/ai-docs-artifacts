#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <stdio.h>

// Replace the following with your MongoDB deployment's connection string
const char *uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";

// Replace <event name> with the name of the event you want to watch
char *event_name = "<event name>";

bool watch_callback (const mongoc_apm_command_started_t *event)
{
    const bson_t *doc;
    char *temp_json;

    // Log the event if it corresponds to the event name
    if (strcmp(mongoc_apm_command_started_get_command_name(event), event_name) == 0) {
        doc = mongoc_apm_command_started_get_command(event);
        temp_json = bson_as_canonical_extended_json (doc, NULL);
        printf ("\nreceived event:\n%s\n", temp_json);
        bson_free(temp_json);
    }
    return true;
}

int main() {
    mongoc_client_t *client;
    mongoc_apm_callbacks_t *callbacks;
    bson_error_t error;

    mongoc_init ();

    client = mongoc_client_new (uri);

    // Establish and verify connection
    bson_t *command = BCON_NEW ("ping", BCON_INT32 (1));
    bson_t reply;
    if (!mongoc_client_command_simple(client, "admin", command, NULL, &reply, &error)) {
        fprintf (stderr, "%s\n", error.message);
        return EXIT_FAILURE;
    }

    printf ("\nConnected successfully!\n");

    // Subscribe to the event
    callbacks = mongoc_apm_callbacks_new ();
    mongoc_apm_set_command_started_cb (callbacks, watch_callback);
    mongoc_client_set_apm_callbacks (client, callbacks, NULL);

    // Cleanup
    bson_destroy (&reply);
    bson_destroy (command);
    mongoc_apm_callbacks_destroy (callbacks);
    mongoc_client_destroy (client);
    
    return EXIT_SUCCESS;
}
