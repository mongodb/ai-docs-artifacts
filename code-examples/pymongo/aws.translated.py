from pymongo import MongoClient
import urllib.parse

# Replace the following with values for your environment.
access_key_id = urllib.parse.quote_plus("<AWS_ACCESS_KEY_ID>")
secret_access_key = urllib.parse.quote_plus("<AWS_SECRET_ACCESS_KEY>")
cluster_url = "<MongoDB cluster url>"

auth_mechanism = "MONGODB-AWS"

uri = (
    f"mongodb+srv://{access_key_id}:{secret_access_key}@{cluster_url}/"
    "?authSource=%24external&authMechanism=" + auth_mechanism
)

# Uncomment the following lines if your AWS authentication setup requires a session token.
# session_token = urllib.parse.quote_plus("<AWS_SESSION_TOKEN>")
# uri += f"&authMechanismProperties=AWS_SESSION_TOKEN:{session_token}"

# Create a new MongoClient
client = MongoClient(uri)

def run():
    try:
        # Establish and verify connection.
        client.admin.command('ping')
        print("Connected successfully to server.")
    finally:
        # Ensure the client closes when it finishes/errors.
        client.close()

try:
  run()
except Exception as e:
  print(e)
