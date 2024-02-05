from pymongo import MongoClient
import pymongo

# Replace the following with your MongoDB deployment's connection string
uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority"

client = MongoClient(uri)

# Replace <event name> with the name of the event you are subscribing to
event_name = "<event name>"

# Create a callback function to be executed when the event occurs
def callback(change):
    print("\nReceived event:\n", change)

# Subscribe to the event
client.watch(callback=callback, full_document='updateLookup')


try:
    # Establish and verify connection
    client.admin.command('ping')
    print("\nConnected successfully!\n")
except pymongo.errors.PyMongoError as e:
    print("\nCould not connect to server:\n", e)
finally:
    # Ensures that the client will close when you finish/error
    client.close()