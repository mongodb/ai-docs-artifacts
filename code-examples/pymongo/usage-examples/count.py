# Count documents in a collection

from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = MongoClient(uri)

try:
    database = client["sample_mflix"]
    movies = database["movies"]

    # Print the estimate of the number of documents in the "movies" collection
    estimate = movies.estimated_document_count()
    print(f"Estimated number of documents in the movies collection: {estimate}")

    # Print the number of documents in the "movies" collection that match the specified query
    query = {"countries": "Canada"}
    countCanada = movies.count_documents(query)
    print(f"Number of movies from Canada: {countCanada}")

finally:
    # Close the connection after the operations complete
    client.close()