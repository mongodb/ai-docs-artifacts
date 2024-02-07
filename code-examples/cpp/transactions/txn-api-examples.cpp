#include <mongocxx/client.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

void coreTest(mongocxx::client& client) {
    auto session = client.start_session();
    try {
        session.start_transaction();

        auto savingsColl = client["bank"]["savings_accounts"];
        savingsColl.update_one(make_document(kvp("account_id", "9876")),
                               make_document(kvp("$inc", make_document(kvp("amount", -100)))),
                               mongocxx::options::update{}.session(session));

        auto checkingColl = client["bank"]["checking_accounts"];
        checkingColl.update_one(make_document(kvp("account_id", "9876")),
                                make_document(kvp("$inc", make_document(kvp("amount", 100)))),
                                mongocxx::options::update{}.session(session));

        // ... perform other operations

        session.commit_transaction();
        std::cout << "Transaction committed." << std::endl;
    }
    catch(mongocxx::exception const& e) {
        std::cout << "An error occurred during the transaction: " << e.what() << std::endl;
        session.abort_transaction();
    }
}

void convTest(mongocxx::client& client) {
    try {
        client.with_session([&](mongocxx::client_session& session) {
            session.with_transaction([&](mongocxx::client_session& session) {
                auto savingsColl = client["bank"]["savings_accounts"];
                savingsColl.update_one(make_document(kvp("account_id", "9876")),
                                       make_document(kvp("$inc", make_document(kvp("amount", -100)))),
                                       mongocxx::options::update{}.session(session));

                auto checkingColl = client["bank"]["checking_accounts"];
                checkingColl.update_one(make_document(kvp("account_id", "9876")),
                                        make_document(kvp("$inc", make_document(kvp("amount", 100)))),
                                        mongocxx::options::update{}.session(session));

                // ... perform other operations

                session.commit_transaction();
                
                std::cout << "Transaction committed." << std::endl;
                return true;
            });
            return true;
        });
    } catch(const mongocxx::exception& e) {
        std::cout << "An error occurred during the transaction: " << e.what() << std::endl;
    }
}

int main(int, char**) {
    mongocxx::uri uri("<connection string>");
    mongocxx::client client(uri);

    coreTest(client);
    convTest(client);
}
