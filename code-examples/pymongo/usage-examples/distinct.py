import pymongo

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = pymongo.MongoClient(uri)

def run():
    try:
        # define a database and collection on which to run the method
        database = client["sample_mflix"]
        movies = database["movies"]

        distinct_values = movies.distinct("year", {
            "directors": "Barbra Streisand",
        })

        print(distinct_values)
    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)