from pymongo import MongoClient

# start-no-error
client = MongoClient()
database = client["<your db>"]
my_coll = database["<your collection>"]
my_coll.update_one({}, {"$set": {"field.nested": "A string"}})
# end-no-error

# start-error
client = MongoClient()
database = client["<your db>"]
my_coll = database["<your collection>"]
my_coll.update_one({}, {"$set": {"field": {"nested": "A string"}}})
# end-error

# start-no-key
client = MongoClient()
database = client["<your database>"]
my_coll = database["<your collection>"]
my_coll.find({"age": "Accepts any type!"})
# end-no-key