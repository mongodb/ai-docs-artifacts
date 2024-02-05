```python
from pymongo import MongoClient, ASCENDING

# Create connection
client = MongoClient()

# Access collection
myColl = client['db_name']['collection_name']

# Start aggregation collation
pipeline = [
    {
        '$group': {
            '_id': '$first_name', 
            'nameCount': { '$sum': 1 }
        }
    },
    {
        '$sort': {
            '_id': ASCENDING
        }
    }
]

myColl.aggregate(pipeline, collation={'locale': 'de@collation=phonebook'})
# End aggregaton collation
```