# Import MongoClient from the PyMongo library
from pymongo import MongoClient

# Replace the placeholders with your credentials
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"

client = MongoClient(uri)

# Connect to the database and the 'movies' collection
database = client['sample_mflix']
movies = database['movies']

# Create a multikey index on the 'cast' field
result = movies.create_index('cast')

# Print the result of creating the index
print(f'Index created: {result}')

# Query to find all movies featuring actor 'Viola Davis'
query = {"cast": "Viola Davis"}

# Projection to return only 'title' and 'cast', excluding '_id'
projection = {"_id": 0, "cast": 1, "title": 1}

# Perform the query with the filter and projection, and print the results
cursor = movies.find(query, projection)
for doc in cursor:
    print(doc)

# Close the connection
client.close()