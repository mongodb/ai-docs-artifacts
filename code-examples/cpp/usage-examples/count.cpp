#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

int main(int, char**) {

    mongocxx::uri uri("<connection string uri>");
    mongocxx::client client(uri);
    mongocxx::database database = client["sample_mflix"];
    mongocxx::collection movies = database["movies"];

    try {
        /* Print the estimate of the number of documents in the
        "movies" collection */
        std::int64_t estimate = movies.estimated_document_count();
        std::cout << "Estimated number of documents in the movies collection: " << estimate << std::endl;

        /* Print the number of documents in the "movies" collection that
        match the specified query */
        bsoncxx::builder::stream::document query_builder;
        query_builder << "countries" << "Canada";
        std::int64_t countCanada = movies.count_documents(query_builder.view());
        std::cout << "Number of movies from Canada: " << countCanada << std::endl;
    } catch(const std::exception& ex) {
        std::cerr << ex.what() << '\n';
    }

    // Client will be automatically cleaned up on destruct
}
