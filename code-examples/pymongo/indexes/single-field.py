# Import MongoClient from pymongo
from pymongo import MongoClient

# Replace the following with your MongoDB deployment's connection string
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"

client = MongoClient(uri)

# Connect to your 'sample_mflix' database and 'movies' collection
database = client['sample_mflix']
movies = database['movies']

# Create an ascending index on the "title" field in the "movies" collection
result = movies.create_index('title')
print(f'Index created: {result}')

# Define the query parameters
query = { 'title': 'Batman' }
sort = [('title', 1)]
projection = { '_id': 0, 'title': 1 }

# Execute the query using the defined parameters
cursor = movies.find(query, projection).sort(sort)

# Close the connection to MongoDB
client.close()