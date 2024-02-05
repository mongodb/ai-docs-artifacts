# python pymongo equivalent
from pymongo import MongoClient, DESCENDING

client = MongoClient()
db = client['database']
myColl = db['myColl']

myColl.find({"year": 1980}).sort("title", DESCENDING)