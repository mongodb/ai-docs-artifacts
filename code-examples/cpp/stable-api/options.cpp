#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/client.hpp>

int main() {
    // begin serverApiVersion
    mongocxx::instance instance{};
    mongocxx::options::client client_options{};
  
    // Replace the placeholders in the connection string uri with your credentials
    mongocxx::uri uri = mongocxx::uri{"mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"};
 
    /* Create a client with options to specify Stable API Version 1, return
        errors for commands outside of the API version, and raise exceptions
        for deprecated commands */
    client_options.server_api(mongocxx::options::server_api(mongocxx::options::server_api::version::k_version_1).strict(true).deprecation_errors(true));
    mongocxx::client client{uri, client_options};

    // end serverApiVersion
    return 0;
}
