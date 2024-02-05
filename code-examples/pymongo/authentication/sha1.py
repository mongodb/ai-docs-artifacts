from pymongo import MongoClient
from urllib.parse import quote_plus

# Replace the following with values for your environment.
username = quote_plus("<username>")
password = quote_plus("<password>")
clusterUrl = "<MongoDB cluster url>"

authMechanism = "SCRAM-SHA-1"

# Replace the following with your MongoDB deployment's connection string.
uri = f"mongodb+srv://{username}:{password}@{clusterUrl}/?authMechanism={authMechanism}"

# Create a new MongoClient
client = MongoClient(uri)

# Function to connect to the server
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