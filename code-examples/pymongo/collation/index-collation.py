from pymongo import MongoClient, ASCENDING

# create a MongoClient to the running mongod instance
client = MongoClient()

# get a reference to the myColl collection
collection = client.myDB.myColl

# create an index on the "title" field with the "en_US" locale collation, specifying ascending ordering of the "title" field
collection.create_index([('title', ASCENDING)], collation={'locale': 'en_US'})