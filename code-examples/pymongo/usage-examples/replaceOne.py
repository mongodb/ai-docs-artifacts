import pymongo
import random

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = pymongo.MongoClient(uri)

def run():
    try:
        database = client["sample_mflix"]
        movies = database.movies

        result = movies.replace_one(
          { "title": { "$regex": "The Cat from" } },
          {
            "title": f"The Cat from Sector {random.randint(1, 1000)}",
          }
        )
        print(f"Modified {result.modified_count} document(s)")
    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)