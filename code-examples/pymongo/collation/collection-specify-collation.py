# Import the necessary library
from pymongo import MongoClient

# Create a MongoClient to the running mongodb instance
client = MongoClient()

# Access the desired database and collection
db = client['myDatabase']
myColl = db['myCollection']

# Set the collation options
collation = {
    'locale': 'is',
    'caseFirst': 'upper',
}

# find documents where "type" field matches "photograph", sorted accordingly
myColl.find({"type": "photograph"}).sort(collation=collation)