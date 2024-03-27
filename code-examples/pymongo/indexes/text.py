from pymongo import MongoClient

# Replace the following with your MongoDB deployment's connection
# string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?w=majority"

client = MongoClient(uri)

try:
    # begin-idx
    # Get the database and collection on which to create the index 
    myDB = client["testDB"]
    myColl = myDB["blogPosts"]

    # Create a text index on the "title" and "body" fields
    result = myColl.create_index(
        [("title", "text"), ("body", "text")],
        default_language="english",
        weights={"body": 10, "title": 3}
    )
    # end-idx
    print(f"Index created: {result}")

    # begin-query
    # Query for documents where body or title contain "life ahead"
    query = { "$text": { "$search": "life ahead" } }
    
    # Show only the title field
    projection = { "_id": 0, "title": 1 }

    # Execute the find operation
    cursor = myColl.find(query, projection)
    # end-query
    for doc in cursor:
        print(doc)
finally:
    client.close()