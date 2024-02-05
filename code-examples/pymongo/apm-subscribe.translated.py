```python
# Subscribe to an event

from pymongo import MongoClient, monitoring
import json

# Replace the following with your MongoDB deployment's connection string
uri = "mongodb+srv://<clusterUrl>/?replicaSet=rs&w=majority"

client = MongoClient(uri)

# Replace <event name> with the name of the event you are subscribing to
event_name = "<event name>"

class CommandLogger(monitoring.CommandListener):
    # Subscribe to a specified event and print a message when the event is received
    def command_started(self, event):
        if event.command_name == event_name:
            print(f'received {event_name}: {json.dumps(event.command, indent=2)}')

monitoring.register(CommandLogger())

def run():
    try:
        # Establish and verify connection to the "admin" database
        client.admin.command('ping')
        print("Connected successfully")
    finally:
        # Close the database connection on completion or error
        client.close()

try:
    run()
except Exception as e:
    print(e)
```