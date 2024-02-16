# Perform an aggregation

from pymongo import MongoClient
import os

uri = os.getenv("MONGDODB_URI")
client = MongoClient(uri)

def run():
    # begin data insertion
    db = client["aggregation"]
    coll = db["restaurants"]

    # Create sample documents
    docs = [
        {"stars": 3, "categories": ["Bakery", "Sandwiches"], "name": "Rising Sun Bakery"},
        {"stars": 4, "categories": ["Bakery", "Cafe", "Bar"], "name": "Cafe au Late"},
        {"stars": 5, "categories": ["Coffee", "Bakery"], "name": "Liz's Coffee Bar"},
        {"stars": 3, "categories": ["Steak", "Seafood"], "name": "Oak Steakhouse"},
        {"stars": 4, "categories": ["Bakery", "Dessert"], "name": "Petit Cookie"},
    ]

    # Insert documents into the restaurants collection
    result = coll.insert_many(docs)
    # end data insertion

    # begin aggregation
    # Define an aggregation pipeline with a match stage and a group stage
    pipeline = [
        {"$match": {"categories": "Bakery"}},
        {"$group": {"_id": "$stars", "count": {"$sum": 1}}},
    ]

    # Execute the aggregation
    agg_cursor = coll.aggregate(pipeline)

    # Print the aggregated results
    for doc in agg_cursor:
        print(doc)
    # end aggregation

# Run the program and print thrown errors, then close the connection
try:
    run()
except Exception as e:
    print(e)
finally:
    client.close()