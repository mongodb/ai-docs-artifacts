from pymongo import MongoClient
import urllib.parse

# Replace the following with values for your environment.
username = urllib.parse.quote_plus('<username>')
password = urllib.parse.quote_plus('<password>')
clusterUrl = '<MongoDB cluster url>'

# Replace the following with your MongoDB deployment's connection string.
uri = f"mongodb+srv://{username}:{password}@{clusterUrl}/?authMechanism=SCRAM-SHA-256&tls=true&tlsCertificateKeyFile=<clientPEMFile>"

# Create a new MongoClient
client = MongoClient(uri)

# Function to connect to the server
def run():
    try:
        # Establish and verify connection
        client["admin"].command("ping")
        print("Connected successfully to server")
    except Exception as e:
        print(e)
    finally:
        # Ensures that the client will close when you finish/error
        client.close()

run()