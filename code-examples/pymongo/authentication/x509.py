from pymongo import MongoClient
import urllib.parse

# Replace the following with values for your environment.
clusterUrl = "<MongoDB cluster url>"
clientPEMFile = urllib.parse.quote("<path to the client pem certificate file>")

authMechanism = "MONGODB-X509"

# Replace the following with your MongoDB deployment's connection string.
uri = f"mongodb+srv://{clusterUrl}/?authMechanism={authMechanism}&tls=true&tlsCertificateKeyFile={clientPEMFile}"

# Create a new MongoClient
client = MongoClient(uri)

# Function to connect to the server
def run():
    try:
        # Establish and verify connection
        client.admin.command('ping')
        print("Connected successfully to server")
    finally:
        # Ensures that the client will close when you finish/error
        client.close()


try:
    run()
except Exception as e:
    print(e)