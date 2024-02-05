from pymongo import MongoClient

uri = "mongodb://localhost:27017/test?maxPoolSize=5000"
# create a new MongoClient
client = MongoClient(uri)

try:
    # connection
    client.connect()
except Exception as err:
    print(err)