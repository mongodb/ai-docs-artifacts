# List indexes
from pymongo import MongoClient

# Replace the placeholders with your credentials
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"

client = MongoClient(uri)

# Access a collection from a database
database = client["<databaseName>"]
collection = database["<collectionName>"]

try:
    # start listIndexes example
    # List the indexes on the collection and output them as an array
    result = collection.index_information()

    # Print the list of indexes
    print("Existing indexes:\n")
    for index in result:
        print(index)
    # end listIndexes example
except Exception as e:
    print(e)
finally:
    # Close the connection after the operation completes
    client.close()