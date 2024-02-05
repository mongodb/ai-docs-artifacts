from pymongo import MongoClient
from pymongo import ReturnDocument

# Assuming you have client
client = MongoClient()

# Access database
db = client['mydatabase']

# Access collection
myColl = db['myColl']

# Create collation dictionary
collation = {'locale': 'de', 'strength': 1}

# Perform update
myColl.find_one_and_update(
    {"first_name": {"$lt": "Gunter"}}, 
    {"$set": {"verified": True}}, 
    collation=collation, 
    return_document=ReturnDocument.AFTER
)
