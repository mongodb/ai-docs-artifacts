import pymongo

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = pymongo.MongoClient(uri)

try:
    # Get the "sample_mflix" database
    db = client["sample_mflix"]

    # Find and print the storage statistics for the "sample_mflix" database using the 'dbStats' command
    result = db.command("dbstats")
    print(result)
except Exception as e:
    print(e)
finally:
    # Close the database connection on completion or error
    client.close()
