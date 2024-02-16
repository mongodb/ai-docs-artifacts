# Delete a document

from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = MongoClient(uri)

def run():
    try:
        database = client["sample_mflix"]
        movies = database["movies"]

        # Delete the first document in the "movies" collection that matches
        # the specified query document
        query = { "title": "Annie Hall" }
        result = movies.delete_one(query)

        # Print a message that indicates whether the operation deleted a
        # document
        if result.deleted_count == 1:
            print("Successfully deleted one document.")
        else:
            print("No documents matched the query. Deleted 0 documents.")
    finally:
        # Close the connection after the operation completes
        client.close()

# Run the program and print any thrown exceptions
try:
    run()
except Exception as e:
    print(e)