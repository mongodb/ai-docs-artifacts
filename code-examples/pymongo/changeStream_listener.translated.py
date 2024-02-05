```python
# Change stream listener

from pymongo import MongoClient
import time
import asyncio

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = MongoClient(uri)

async def simulate_async_pause():
    await asyncio.sleep(1)

async def run():
    database = client['insertDB']
    haikus = database['haikus']

    # Open a Change Stream on the "haikus" collection
    change_stream = haikus.watch()

    # Set up a change stream listener when change events are emitted
    async for change in change_stream:
        # Print any change event
        print(f"received a change to the collection: \t {change}")

    # Pause before inserting a document
    await simulate_async_pause()

    # Insert a new document into the collection
    await haikus.insert_one({
        'title': 'Record of a Shriveled Datum',
        'content': 'No bytes, no problem. Just insert a document, in MongoDB'
    })

    # Pause before closing the change stream
    await simulate_async_pause()

    # Close the change stream
    change_stream.close()
    print("closed the change stream")

    # Close the database connection on completion or error
    client.close()

loop = asyncio.get_event_loop()
try:
    loop.run_until_complete(run())
except Exception as e:
    print(e)
finally:
    loop.close()
```
Please note that change streams and some other features are available only in MongoDB 3.6 and up. Therefore this Python code works for version 3.6 and up. If you try this with an earlier version of MongoDB, you may receive an error.