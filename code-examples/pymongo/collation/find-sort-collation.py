from pymongo import MongoClient

# Create a client connection to the MongoDB server
client = MongoClient('localhost', 27017)

# Connect to the desired database
db = client['myDb']

# Connect to the desired collection
myColl = db['myColl']

# Retrieve documents with "city" field equals "New York"
# sorted by the "name" field
# A German ('de') collation is used for sorting
result = myColl.find({"city": "New York"}, collation={'locale': 'de'}).sort("name")