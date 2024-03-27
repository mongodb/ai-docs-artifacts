from pymongo import MongoClient

# Replace the placeholder with your Atlas connection string
uri = "<connection string>"

# Create a MongoClient 
client = MongoClient(uri)

def run():
    try:
        # Connect the client to the server
        client.server_info()

        # Send a ping to confirm a successful connection
        client.admin.command('ping')
        print("Pinged your deployment. You successfully connected to MongoDB!")
    except Exception as e:
        print(e)
    finally:
        # Ensures that the client will close when you finish/error
        client.close()

run()