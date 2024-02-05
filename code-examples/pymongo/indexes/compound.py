from pymongo import MongoClient, ASCENDING

# Replace the following with your MongoDB deployment's connection string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?w=majority"
client = MongoClient(uri)

try:
    # Connect to the "sample_mflix" database
    database = client["sample_mflix"]
    # Access the database's "movies" collection
    movies = database["movies"]

    # Create an ascending index on the "type" and "genre" fields in the "movies" collection.
    result = movies.create_index([("type", ASCENDING), ("genre", ASCENDING)])
    print(f"Index created: {result}")

    # Define a query to find movies in the "Drama" genre
    query = {"type": "movie", "genre": "Drama"}
    # Define sorting criteria for the query results
    sort = [("type", ASCENDING), ("genre", ASCENDING)]
    # Include only the type and genre fields in the query results
    projection = {"_id": 0, "type": 1, "genre": 1}

    # Execute the query using the defined criteria and projection
    cursor = movies.find(query, projection).sort(sort)

except Exception as e:
    print(e)

finally:
    # Close the MongoDB client connection
    client.close()