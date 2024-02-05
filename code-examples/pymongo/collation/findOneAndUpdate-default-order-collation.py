# Import pymongo module to access MongoDB functions
from pymongo import MongoClient

# Creating a MongoClient instance and connecting to the database
client = MongoClient()
db = client['database']
myColl = db['collection']

# Use pymongo's find_one_and_update to update the "verified" field of the first document
# that comes before "Gunter" in default binary collation order
myColl.find_one_and_update(
  { "first_name" : { "$lt": "Gunter" } },
  { "$set": { "verified": True } }
)