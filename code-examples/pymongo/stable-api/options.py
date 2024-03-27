# Stable API - with options

# begin serverApiVersion
from pymongo import MongoClient, WriteConcern

# Replace the placeholders in the connection string uri with your credentials
uri = "mongodb+srv://<user>:<password>@<cluster-url>"

# Create a client with options to specify Stable API Version 1, return
# errors for commands outside of the API version, and raise exceptions
# for deprecated commands
client = MongoClient(uri, serverSelectionTimeoutMS=5000, serverApi="1", writeConcern=WriteConcern(w="majority"))
# end serverApiVersion