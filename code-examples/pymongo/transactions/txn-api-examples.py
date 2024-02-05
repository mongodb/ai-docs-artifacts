from pymongo import MongoClient
from pymongo.errors import PyMongoError

# begin-core
def core_test(client):
    session = client.start_session()
    try:
        session.start_transaction()

        savings_coll = client["bank"]["savings_accounts"]
        savings_coll.update_one(
            {"account_id": "9876"}, 
            {"$inc": {"amount": -100}}, 
            session=session)

        checking_coll = client["bank"]["checking_accounts"]
        checking_coll.update_one(
            {"account_id": "9876"}, 
            {"$inc": {"amount": 100}}, 
            session=session)

        # ... perform other operations

        session.commit_transaction()
        print("Transaction committed.")
    except PyMongoError as error:
        print("An error occurred during the transaction:", error)
        session.abort_transaction()
    finally:
        session.end_session()
# end-core

# begin-conv
def conv_test(client):
    with client.start_session() as session:
        try:
            session.start_transaction()

            savings_coll = client["bank"]["savings_accounts"]
            savings_coll.update_one(
                {"account_id": "9876"}, 
                {"$inc": {"amount": -100}}, 
                session=session)
      
            checking_coll = client["bank"]["checking_accounts"]
            checking_coll.update_one(
                {"account_id": "9876"}, 
                {"$inc": {"amount": 100}}, 
                session=session)

            # ... perform other operations

            session.commit_transaction()
            print("Transaction committed.")

        except PyMongoError as error:
            print("An error occurred during the transaction:", error)
            session.abort_transaction()
# end-conv

def run():
    uri = "<connection string>"
    client = MongoClient(uri)

    try:
        core_test(client)
        conv_test(client)
    finally:
        client.close()
run()
