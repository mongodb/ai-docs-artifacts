```python
# Update multiple documents

from pymongo import MongoClient
from random import random

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

class Rating:
    G = "G"
    PG = "PG"
    PG_13 = "PG-13"
    R = "R"
    NR = "NOT RATED"
    
def run():
    try:
        # Get the "movies" collection in the "sample_mflix" database
        db = client["sample_mflix"]
        movies = db["movies"]
        
        # Update all documents that match the specified filter
        result = movies.update_many(
            {'rated': Rating.G},
            {
                "$set": {
                    'random_review': f"After viewing I am {100 * random()}% more satisfied with life.",
                },
            }
        )
        print(f"Updated {result.modified_count} document(s)")
        
    finally:
        # Close the database connection on completion or error
        client.close()

if __name__ == "__main__":
    run()
```