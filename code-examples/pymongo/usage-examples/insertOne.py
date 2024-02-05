from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

class Haiku:
    def __init__(self, title, content):
        self.title = title
        self.content = content

def run():
    try:
        database = client["insertDB"]
        haiku = database["haiku"]
        haiku_document = Haiku("Record of a Shriveled Datum", "No bytes, no problem. Just insert a document, in MongoDB")
        result = haiku.insert_one(haiku_document.__dict__)
        print(f"A document was inserted with the _id: {result.inserted_id}")
    finally:
        client.close()

try:
    run()
except Exception as e:
    print(e)