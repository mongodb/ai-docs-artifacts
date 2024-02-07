#include <mongoc/mongoc.h>
#include <bson/bson.h>

void log_sdam_event(const mongoc_apm_server_changed_t *event)
{
    bson_t *event_as_bson = mongoc_apm_server_changed_get_topology_description(event);
    char *event_as_string = bson_as_json(event_as_bson, NULL);

    printf("received %s: %s", mongoc_apm_server_changed_get_host(event)->host_and_port, event_as_string);

    bson_free(event_as_string);
}

int main()
{
    mongoc_client_t *client;
    mongoc_uri_t *uri;
    mongoc_apm_callbacks_t *callbacks;

    mongoc_init();

    // Replace the following with your MongoDB deployment's connection string
    uri = mongoc_uri_new("mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority");
    client = mongoc_client_new_from_uri(uri);

    callbacks = mongoc_apm_callbacks_new();
    mongoc_apm_set_server_changed_cb(callbacks, log_sdam_event);

    mongoc_client_set_apm_callbacks(client, callbacks, NULL);

    // Establish and verify connection to the database
    if (mongoc_client_command_simple(client, "admin", BCON_NEW("ping", BCON_INT32(1)), NULL, NULL, NULL)) {
        printf("Connected successfully");
    } else {
        fprintf(stderr, "Connection failed");
    }

    // Close the database connection on completion or error
    mongoc_uri_destroy(uri);
    mongoc_client_destroy(client);
    mongoc_apm_callbacks_destroy(callbacks);

    mongoc_cleanup();

    return 0;
}
