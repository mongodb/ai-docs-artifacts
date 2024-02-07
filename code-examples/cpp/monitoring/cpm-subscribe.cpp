#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/stdx.hpp>

class Logger : public mongocxx::logger {
public:
   void operator()(mongocxx::log_level level, mongocxx::stdx::string_view domain,
                   mongocxx::stdx::string_view message) noexcept override {
       std::cout << "Log message: " << message.to_string() << std::endl;
   }
};

int main() {
   mongocxx::instance instance{};
   Logger logger{};
   
   // Replace the following with your MongoDB deployment's connection string
   mongocxx::uri uri = ...;

   mongocxx::options::client options{};
   options.apm_opts().on_command_started(
       [](mongocxx::events::command_started_event event) {
           std::cout << "\nCommand started:\n"
                     << bsoncxx::to_json(event.command()) << std::endl;
       });

   mongocxx::client client{uri, options};

   try {
       // Establish and verify connection
       auto admin = client["admin"];
       bsoncxx::builder::stream::document document{};
       document << "ping" << 1;

       admin.run_command(document.view());
       std::cout << "\nConnected successfully!\n";
   } catch (const mongocxx::exception& e) {
       std::cout << "An exception occurred: " << e.what() << "\n";
       return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
