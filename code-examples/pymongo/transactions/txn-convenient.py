from pymongo import MongoClient, ReturnDocument
from bson.objectid import ObjectId

uri = "<connection string uri>"
client = MongoClient(uri)

db = client["testdb"]

inventory = db["inventory"]
inventory.insert_many([
    {"item": "sunblock", "qty": 85, "price": 6.0},
    {"item": "beach chair", "qty": 30, "price": 25.0}
])

order1 = [
  {"item": "sunblock", "qty": 3},
  {"item": "beach chair", "qty": 1}
]

order2 = [
  {"item": "volleyball", "qty": 1}
]

order = order1

with client.start_session() as session:
    session.start_transaction():
    
    orders = db["orders"]
    total = 0
    for item in order:
        in_stock = inventory.find_one_and_update(
            {"item": item["item"], "qty": {"$gte": item["qty"]}},
            {"$inc": {"qty": -item["qty"]}},
            return_document=ReturnDocument.AFTER,
            session=session
        )
        if in_stock is None:
            session.abort_transaction()
            print("Item not found or insufficient quantity.")
            break
        sub_total = item["qty"] * in_stock["price"]
        total += sub_total

    else:
        receipt = {
            "date": datetime.datetime.utcnow(),
            "items": order,
            "total": total
        }
        orders.insert_one(receipt, session=session)
        print("Order successfully completed and recorded!\nReceipt:\n")
        print(receipt)
    
    session.commit_transaction()
    
client.close()