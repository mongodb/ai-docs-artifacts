from pymongo import MongoClient
from pymongo.collation import Collation

client = MongoClient('localhost', 27017)
db = client['database_name'] 
myColl = db['myColl']

myColl.find({"year": 1980}).sort("title", -1).collation(Collation(locale="en_US", strength=2))