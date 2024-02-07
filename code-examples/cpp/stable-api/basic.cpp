#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/stdx/make_unique.hpp>
#include <bsoncxx/builder/stream/document.hpp>

// Replace the placeholders in the connection string uri with your credentials
std::string uriString = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";

// Create options with server API version
auto serverApi = bsoncxx::stdx::make_unique<mongocxx::options::server_api>(
    mongocxx::options::server_api::version::k_version_1);
auto uri = mongocxx::uri { uriString };
mongocxx::options::client client_opts;
client_opts.server_api_opts(std::move(serverApi));

// Using client object for the MongoDB connection
mongocxx::client client {uri, client_opts};
