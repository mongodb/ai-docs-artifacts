from pymongo import MongoClient

# Create a MongoDB client instance
client = MongoClient()

# Access the database
db = client['database_name']  # Replace 'database_name' with the actual name of your MongoDB database

# Create the "souvenirs" collection and specify the French Canadian collation
db.create_collection("souvenirs", collation={'locale': 'fr_CA'})