from pymongo import MongoClient, DESCENDING

# create a MongoClient to the running mongod instance
client = MongoClient()

# select a database to use
db = client['mydatabase']

# select a collection within the database
coll = db['myColl']

# find documents where "year" equals "1980", sort by "title" in descending order with "en_US" collation
docs = coll.find({"year": 1980}).sort("title", DESCENDING).collation({"locale": "en_US"})

# fetch and print all documents from the cursor
for doc in docs:
    print(doc)