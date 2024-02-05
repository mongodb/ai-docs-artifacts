from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

# Create a new client and connect to MongoDB
client = MongoClient(uri)

# Create a database object to the "testDB" database
db = client['testDB']

try:
    # Run a command to check metadata consistency within the database
    command_result = db.command({"validate": "testDB", "full": True})

    if 'errors' in command_result or 'warnings' in command_result:
        if 'errors' in command_result:
            for error in command_result['errors']:
                print(error)
        if 'warnings' in command_result:
            for warning in command_result['warnings']:
                print(warning)

except Exception as e:
    print(e)
finally:
    # Close the MongoDB client connection
    client.close()