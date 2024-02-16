from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

class Food(object):
    def __init__(self, name, healthy):
        self.name = name
        self.healthy = healthy

def run():
    try:
        database = client["insertDB"]
        foods = database["foods"]

        result = foods.insert_many(
            [
                Food("cake", False).__dict__,
                Food("lettuce", True).__dict__,
                Food("donut", False).__dict__,
            ]
        )
        print(f'{result.inserted_count} documents were inserted')
    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)