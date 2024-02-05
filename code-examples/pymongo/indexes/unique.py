# Create unique index
from pymongo import MongoClient 

# Replace the following with your MongoDB deployment's connection string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority" 

# Create a client
client = MongoClient(uri)

try:
    # Begin index
    database = client["sample_mflix"]
    movies = database["movies"]

    # Create a unique index on the "theaterId" field in the "theaters" collection.
    result = movies.create_index("theaterId", unique=True)
    print(f"Index created: {result}")
    # End index

# Close the database connection
finally:
    client.close()