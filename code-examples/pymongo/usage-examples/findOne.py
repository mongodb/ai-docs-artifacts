from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

def run():
    try:
        database = client["sample_mflix"]
        movies = database["movies"]

        movie = movies.find_one(
            {"title": "The Room"}, 
            sort=[("rating", -1)], 
            projection={"_id": 0, "title": 1, "imdb": 1}
        )
        print(movie)
    finally:
        client.close()

if __name__ == "__main__":
    try:
        run()
    except Exception as e:
        print(e)