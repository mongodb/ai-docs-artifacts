# Cursor-based document retrieval
from pymongo import MongoClient

# Replace the following string with your MongoDB deployment's connection string
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"
client = MongoClient(uri)

# Asynchronous iteration using cursor methods
# Python does not have native support for asynchronous iteration over a cursor.
# This is an equivalent synchronous iteration.
def async_iteration(my_coll):
    cursor = my_coll.find({})
    print("async")
    for doc in cursor:
        print(doc)
    cursor.close()


# Manual iteration using cursor methods
def manual_iteration(my_coll):
    cursor = my_coll.find({})
    for doc in cursor:
        print(doc)
    cursor.close()


# Stream API does not exist in pymongo, but we can iterate through the cursor as with an event-base API
def stream_api(my_coll):
    cursor = my_coll.find({})
    for doc in cursor:
        print(doc)
    cursor.close()

# Event-based iteration
# Python doesn't have events like Node.js, a for loop can be used to mimic the functionality
def event_api(my_coll):
    cursor = my_coll.find({})
    for doc in cursor:
        print(doc)
    cursor.close()


# Fetch all documents as an array using the `to_list` method
def fetch_all(my_coll):
    cursor = my_coll.find({})
    all_values = list(cursor)
    print(len(all_values))
    cursor.close()


# Rewind the cursor
# Pymongo cursors cannot be rewound after starting iteration. You need to recreate cursor to reuse it.
def rewind(my_coll):
    cursor = my_coll.find({})
    first_result = list(cursor)
    print("First count: " + str(len(first_result)))
    cursor = my_coll.find({})
    second_result = list(cursor)
    print("Second count: " + str(len(second_result)))


# Close a cursor
# This is implicitly done when cursor is exhausted or when `cursor.close()` is called.
def close(my_coll):
    cursor = my_coll.find({})
    cursor.close()


def run():
    try:
        database = client["test"]
        orders = database["orders"]

        async_iteration(orders)
        manual_iteration(orders)
        stream_api(orders)
        event_api(orders)
        fetch_all(orders)
        rewind(orders)
        close(orders)
    finally:
        client.close()

run()
