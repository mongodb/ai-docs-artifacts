"""
Delete the first document that contains a value greater
than 100 in the "a" field when ordered by using the
English numeric collation order.
"""
from pymongo import MongoClient
from bson import SON

client = MongoClient()
db = client.myDb
myColl = db.myColl

# PyMongo doesn't natively support collation in queries, so we can use 
# the aggregation pipeline with a $match and $limit stage instead

pipeline = [
    {"$match": {"a": {"$gt": 100}}},
    {"$sort": SON([("a", 1)])},
    {"$limit": 1}
]

result = myColl.aggregate(pipeline)

# Delete the document
for doc in result:
    myColl.delete_one(doc)