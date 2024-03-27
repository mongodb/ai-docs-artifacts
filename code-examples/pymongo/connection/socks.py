import pymongo
import socks
import socket
from getpass import getpass

# Replace the placeholders with your SOCKS5 proxy server details
socks.set_default_proxy(socks.PROXY_TYPE_SOCKS5, "<host>", 1080, 
                        username="<username>", password="<password>")
socket.socket = socks.socksocket

# Replace the placeholder with your connection string
client = pymongo.MongoClient("<connection string uri>")

try:
    # Connecting to a database
    db = client['myDB']
    
    # Selecting a collection
    my_coll = db['myColl']
    
    # Fetching the first document
    first_doc = my_coll.find_one()

    # Print the retrieved document
    print(first_doc)

except Exception as e:
    print(e)

finally:
    # Close the connection after the operation completes
    client.close()