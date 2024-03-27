from pymongo import MongoClient
import os

# Remember to specify your AWS credentials in environment variables.
clusterUrl = "<MongoDB deployment url>"
authMechanism = "MONGODB-AWS"

uri = f"mongodb+srv://{clusterUrl}/?authSource=%24external&authMechanism={authMechanism}"

# Create a new MongoClient.
client = MongoClient(uri)

def run():
    try:
        # Establish and verify connection.
        client.admin.command("ping")
        print("Connected successfully to server.")
    except Exception as e:
        print(e)
    finally:
        # Ensure the client closes when it finishes/errors.
        client.close()

run()