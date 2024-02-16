import pymongo
from pymongo import MongoClient

# Replace the uri string with your MongoDB deployment's connection string.
uri = "<connection string uri>"

client = MongoClient(uri)

class Address:
    street1: str
    city: str
    state: str
    zipcode: str

class Theater:
    location: {"address": Address}
    is_in_ohio: bool = None

async def run():
    try:
        database = client["sample_mflix"]
        theaters = database["theaters"]

        operations = [
            pymongo.InsertOne(
                {
                    "location": {
                        "address": {
                            "street1": "3 Main St.",
                            "city": "Anchorage",
                            "state": "AK",
                            "zipcode": "99501",
                        }
                    },
                }
            ),
            pymongo.InsertOne(
                {
                    "location": {
                        "address": {
                            "street1": "75 Penn Plaza",
                            "city": "New York",
                            "state": "NY",
                            "zipcode": "10001",
                        }
                    },
                }
            ),
            pymongo.UpdateMany(
                {"location.address.zipcode": "44011"},
                {"$set": {"is_in_ohio": True}},
                upsert=True,
            ),
            pymongo.DeleteOne({"location.address.street1": "221b Baker St"}),
        ]

        result = await theaters.bulk_write(operations)

        print(result)

    finally:
        await client.close()

run().catch(print)