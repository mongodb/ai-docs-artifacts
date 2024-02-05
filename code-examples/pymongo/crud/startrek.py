```python
from pymongo import MongoClient
from bson.son import SON

# Replace the following string with your MongoDB deployment's connection string
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"
client = MongoClient(uri)

def word(movies):
    query = {'$text': {'$search': "trek"}}
    projection = {'_id': 0, 'title': 1}
    cursor = movies.find(query, projection)

    if cursor.count() == 0:
        print("No documents found!")
    else:
        for doc in cursor:
            print(doc)

def phrase(movies):
    query = {'$text': {'$search': "\"star trek\""}}
    projection = {'_id': 0, 'title': 1}
    cursor = movies.find(query, projection)

    if cursor.count() == 0:
        print("No documents found!")
    else:
        for doc in cursor:
            print(doc)

def negation(movies):
    query = {'$text': {'$search': "\"star trek\" -\"into darkness\""}}
    projection = {'_id': 0, 'title': 1}
    cursor = movies.find(query, projection)

    if cursor.count() == 0:
        print("No documents found!")
    else:
        for doc in cursor:
            print(doc)

def relevance(movies):
    query = {'$text': {'$search': "\"star trek\" -\"into darkness\""}}
    projection = {'_id': 0, 'title': 1, 'score': {'$meta': "textScore"}}
    cursor = movies.find(query, projection).sort([('score', {'$meta': 'textScore'})])

    if cursor.count() == 0:
        print("No documents found!")
    else:
        for doc in cursor:
            print(doc)

def run():
    db = client["sample_mflix"]
    movies = db["movies"]

    try:
        word(movies)
        phrase(movies)
        negation(movies)
        relevance(movies)
    finally:
        client.close()

if __name__ == "__main__":
    run()
```