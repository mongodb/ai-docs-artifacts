# Create a geospatial index

from pymongo import MongoClient

# Replace the placeholders with your MongoDB deployment's credentials
uri = 'mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority'

client = MongoClient(uri)

try:
    # begin-idx
    database = client["sample_mflix"]
    movies = database["movies"]

    # Create a 2dsphere index on the "location.geo" field in the
    # "movies" collection
    result = movies.create_index([("location.geo", "2dsphere")])

    # Print the result of the index creation
    print(f'Index created: {result}')
    # end-idx

finally:
    # Close the connection after the operation completes
    client.close()