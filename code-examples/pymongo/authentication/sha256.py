from pymongo import MongoClient
from urllib.parse import quote_plus

# Replace the following with values for your environment.
username = quote_plus("<username>")
password = quote_plus("<password>")
cluster_url = "<MongoDB cluster url>"

auth_mechanism = "SCRAM-SHA-256"

# Replace the following with your MongoDB deployment's connection string.
uri = f"mongodb+srv://{username}:{password}@{cluster_url}/?authMechanism={auth_mechanism}"

# Create a new MongoClient
client = MongoClient(uri)

def run():
    try:
        # Establish and verify connection
        client.admin.command('ping')
        print("Connected successfully to server")
    except Exception as e:
        print(e)
    finally:
        # Ensures that the client will close when you finish/error
        client.close()

run()