# Geospatial queries using proximity to a location and a specific geographic range
from pymongo import MongoClient
import pprint

# Replace the following string with your MongoDB deployment's connection string.
uri = "mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority"
client = MongoClient(uri)

def proximity(theaters):
    # Define the query to find theaters near a specific location
    query = {
        "location.geo": {
            "$near": {
                "$geometry": {"type": "Point", "coordinates": [-73.9667, 40.78]},
                "$maxDistance": 10000,
            },
        },
    }

    # Find documents based on our query
    cursor = theaters.find(query)
        
    # Print a message if no documents were found
    if theaters.count_documents(query) == 0:
        print("No documents found!")
    
    # Iterate through the found documents and display them
    for doc in cursor:
        pprint.pprint(doc)
        

def range_query(theaters):
    # Define the query to find theaters within a specified polygon
    query = {
        "location.geo": {
            "$geoWithin": {
                "$geometry": {
                    "type": "Polygon",
                    "coordinates": [
                        [
                            [-72, 40],  # Polygon coordinates defining the range
                            [-74, 41],
                            [-72, 39],
                            [-72, 40],
                        ],
                    ],
                },
            },
        },
    }
    # Find documents based on our query
    cursor = theaters.find(query)
       
    # Print a message if no documents were found
    if theaters.count_documents(query) == 0:
        print("No documents found!")
    
    # Iterate through the found documents and display them
    for doc in cursor:
        pprint.pprint(doc)


try:
    database = client["sample_mflix"]
    theaters = database["theaters"]

    proximity(theaters)
    range_query(theaters)
finally:
    client.close()