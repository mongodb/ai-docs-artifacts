# Stable API - no options

# begin serverApiVersion
from pymongo import MongoClient, server_api

# Replace the placeholders in the connection string uri with your credentials
uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority"

# Create a client with options to specify Stable API Version 1
client = MongoClient(uri, server_api=server_api.ServerApi('1'))
# end serverApiVersion