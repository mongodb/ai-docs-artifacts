# Import PyMongo module
from pymongo import MongoClient

# Replace the following with your MongoDB deployment's connection string
uri = "mongodb+srv://<user>:<password>@<cluster-url>?w=majority"

client = MongoClient(uri)

def run():
    try:
        database = client["<databaseName>"]
        collection = database["<collectionName>"]

        # start createSearchIndex example
        # Create a search index
        index1 = {
            'name': 'search1',
            'key': {
                '$**': 'text'
            }
        }
        collection.create_index([(k, v) for k, v in index1.items()])
        # end createSearchIndex example

        # start listSearchIndexes example
        # List search indexes
        result = list(collection.list_indexes())
        print('Existing search indexes:\n')
        for doc in result:
            print(doc)
        # end listSearchIndexes example

        # start dropSearchIndex example
        # Dropping (deleting) a search index
        collection.drop_index('search1_$**_text')
        # end dropSearchIndex example

    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)