# Subscribe to SDAM event

from pymongo import MongoClient
from bson.json_util import dumps

# Replace the following with your MongoDB deployment's connection string
uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&w=majority"

client = MongoClient(uri)

# Replace <event name> with the name of the event you are subscribing to
event_name = "<event name>"

class MyListener(object):

    def __init__(self, event_name):
        self.event_name = event_name

    def received_sdmevent(self, event):
        print(f'received {self.event_name}: {dumps(event)}')

listener = MyListener(event_name)
client.add_sdam_listener(listener.received_sdmevent)

try:
    # Establish and verify connection to the database
    client.admin.command('ping')
    print("Connected successfully")

except Exception as e:
    print(str(e))

finally:
    # Close the database connection on completion or error
    client.close()
