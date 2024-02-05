from pymongo import MongoClient, errors
from getpass import getpass
import os

# Deletes the "customers", "inventory", and "orders" collections from the "testdb" database
def clean_up(client):
    for col in ['customers', 'inventory', 'orders']:
        try:
            client['testdb'][col].drop()
        except:
            pass

def setup(client):
    try:
        customer_col = client['testdb']['customers']
        inventory_col = client['testdb']['inventory']

        # Insert order data for customer "98765" in the customers collection
        customer_col.insert_one({'_id': 98765, 'orders': []})

        # Insert inventory data for "sunblock" and "beach towel"
        inventory_col.insert_many([
          {'item': 'sunblock', 'item_id': 5432, 'qty': 85 },
          {'item': 'beach towel', 'item_id': 7865, 'qty': 41 }
        ])
    except Exception as e:
        print('Unable to insert test data: ' + str(e))

# Prints all documents in the "customers", "inventory", and "orders" collections
def query_data(uri):
    print("Query data")
    client = MongoClient(uri)
    for c in ['customers', 'inventory', 'orders']:
        print(list(client['testdb'][c].find()))
    client.close()

# Start placing order
def place_order(client, cart, payment):
    session = client.start_session()

    try:
        # Start the transaction
        session.start_transaction(snapshot=True)

        order_col = client['testdb']['orders']
        order_result = order_col.insert_one(
              {'customer': payment['customer'],
               'items': cart,
               'total': payment['total']
              }, session=session)
        
        inventory_col = client['testdb']['inventory']

        for item in cart:  
          in_stock = inventory_col.find_one_and_update(
                {'item_id': item['item_id'],
                 'qty': { '$gte': item['qty'] }},
                {'$inc': { 'qty': -item['qty'] }},
                session=session
          )
          if in_stock is None:
            raise Exception('Insufficient quantity or item ID not found.')

        customer_col = client['testdb']['customers']

        # Add the order details to the "orders" array of the customer document
        customer_col.update_one(
          { '_id': payment['customer'] },
          { '$push': { 'orders': order_result.inserted_id }},
          session=session
        )

        # Commit and print message
        session.commit_transaction()
        print('Transaction committed successfully.')
        
    # capture and handle exceptions
    except errors.PyMongoError as error:  
        print('An error occurred, transaction will be aborted: ', error)
        session.abort_transaction()
    
    finally:
        session.end_session()

def run():
    uri = os.environ.get('MONGODB_URI')
    client = MongoClient(uri)
  
    clean_up(client)
  
    setup(client)

    cart = [{'item': 'sunblock', 'item_id': 5432, 'qty': 1, 'price': 5.19 },
            {'item': 'beach towel', 'item_id': 7865, 'qty': 2, 'price': 15.99 }]
    
    payment = {'customer': 98765, 'total': 37.17 }

    try:
        place_order(client, cart, payment)
    except:
        pass
    finally:
        clean_up(client)
        client.close()

run()
query_data(os.environ.get('MONGODB_URI'))