# Watch for changes in a collection by using a change stream
from pymongo import MongoClient
from pymongo.errors import PyMongoError

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

# Declare a variable to hold the change stream
change_stream = None

# Define a function to manage the change stream
def run():
    try:
        database = client['insertDB']
        haikus = database['haikus']

        # Open a Change Stream on the "haikus" collection
        change_stream = haikus.watch()

        # Print change events as they occur
        for change in change_stream:
            print("Received change:\n", change)
            
    except PyMongoError as error:
        print(error)
        
    finally:
        # Close the MongoDB client connection and the change stream when done
        if change_stream is not None:
            change_stream.close()
        client.close()

run()