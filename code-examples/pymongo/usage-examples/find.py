import pymongo
from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

class IMDB:
    def __init__(self, rating, votes, id):
        self.rating = rating
        self.votes = votes
        self.id = id

class Movie:
    def __init__(self, title, imdb, runtime):
        self.title = title
        self.imdb = imdb
        self.runtime = runtime

def run():
    try:
        database = client["sample_mflix"]
        movies = database["movies"]

        query = { "runtime": { "$lt": 15 } }
        cursor = movies.find(query).sort("title", 1)

        if cursor.count() == 0:
            print("Warning: No documents found!")

        for doc in cursor:
            print(doc)

    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)