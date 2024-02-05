from pymongo import MongoClient

# Replace the following string with your MongoDB deployment's connection string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"
client = MongoClient(uri)

def printData():
  try:
    myDB = client.get_database("test")
    myColl = myDB.get_collection("testColl")

    documents = myColl.find()
    for doc in documents:
        print(doc)
  finally:
    client.close()


def runFirstArrayElement():
  try:
    myDB = client.get_database("test")
    myColl = myDB.get_collection("testColl")

    documents = myColl.find()
    for doc in documents:
        print(doc)

    query = { "entries.x": { "$type" : "string" } }
    updateDocument = { "$inc": { "entries.$.y": 33 } }

    result = myColl.update_one(query, updateDocument)
    print(result.modified_count)

    documents = myColl.find()
    for doc in documents:
        print(doc)
  finally:
    client.close()


def runAllArrayElements():
  try:
    myDB = client.get_database("test")
    myColl = myDB.get_collection("testColl")

    documents = myColl.find()
    for doc in documents:
        print(doc)

    query = { "date": "5/15/2023" }
    updateDocument = { "$unset": { "calls.$[].duration": "" } }

    result = myColl.update_one(query, updateDocument)
    print(result.modified_count)

    documents = myColl.find()
    for doc in documents:
        print(doc)
  finally:
    client.close()


def arrayFiltersIdentifier():
  try:
    myDB = client.get_database("test")
    myColl = myDB.get_collection("testColl")

    documents = myColl.find()
    for doc in documents:
        print(doc)

    query = { "date": "11/12/2023" }
    updateDocument = { "$mul": { "items.$[i].quantity": 2 } }

    options = [
        {
          "i.recipe": "Fried rice",
          "i.item": { "$not": { "$regex": "oil" } },
        }
    ]

    updateWithOptions = myColl.update_one(query, updateDocument, array_filters=options)
    print(updateWithOptions.modified_count)

    documents = myColl.find()
    for doc in documents:
        print(doc)
  finally:
    client.close()