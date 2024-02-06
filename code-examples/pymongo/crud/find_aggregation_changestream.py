# CRUD (Create, Read, Update, Delete) operations 
from pymongo import MongoClient
from datetime import datetime, timedelta

# Replace the following string with your MongoDB deployment's connection string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"
client = MongoClient(uri)

db = client.test
orders = db.orders

# Searching for orders
current_date = datetime.now()
filter = {
  "name": "Lemony Snicket",
  "date": {
    "$gte": current_date.replace(hour=0, minute=0, second=0),
    "$lt": current_date.replace(hour=23, minute=59, second=59),
  },
}
response = orders.find(filter)
print(list(response))

# Grouping orders by status
pipeline = [
  {
    "$match": {
      "date": {
        "$gte": datetime.now() - timedelta(days=7),
        "$lt": datetime.now(),
      },
    },
  },
  {
    "$group": {
      "_id": "$status",
      "count": {
        "$sum": 1,
      },
    },
  },
]
response = orders.aggregate(pipeline)
print(list(response))

# Watching for new order insertions
watch_filter = [
  {"$match": {"operationType": "insert"}},
  {"$project": {"fullDocument.name": 1, "fullDocument.address": 1}}
]
with orders.watch(watch_filter) as stream:
  for change in stream:
    print("New order for {0} at {1}".format(change['fullDocument']['name'], change['fullDocument']['address']))

# Inserting a new order
new_order = {
  "date": datetime.now(),
  "address": "667 Dark Avenue, San Francisco, CA, 94110",
  "name": "Lemony Snicket",
  "items": [
    {
      "name": "pizza",
      "toppings": ["pepperoni", "mushrooms"],
      "notes": "extra shrooms",
      "price": 11,
    },
    {
      "name": "wings",
      "sauce": "buffalo",
      "notes": "extra crispy",
      "price": 10,
    },
  ],
  "status": "created",
}
result = orders.insert_one(new_order)
print(result.inserted_id) 

# Updating a desired order's address
order_filter = {
  "address": "667 Dark Avenue, San Francisco, CA, 94110",
  "date": {
    "$gte": current_date.replace(hour=0, minute=0, second=0),
    "$lt": current_date.replace(hour=23, minute=59, second=59),
  },
}
update = {"$set": {"address": "667 Dark Avenue, San Francisco, CA, 94103"}}
result = orders.update_one(order_filter, update)
print(result.modified_count) 

# Deleting an order
delete_filter = {
  "address": "13 Lousy Lane",
  "name": "Violet Baudelaire",
  "date": {
    "$gte": current_date.replace(hour=0, minute=0, second=0),
    "$lt": current_date.replace(hour=23, minute=59, second=59),
  },
}
result = orders.delete_one(delete_filter)
print(result.deleted_count) 

client.close()