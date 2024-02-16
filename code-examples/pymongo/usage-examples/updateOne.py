# Update a document

from pymongo import MongoClient
import random 

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = MongoClient(uri)

def run():
    try:
       
        database = client["sample_mflix"]
        movies = database["movies"]

        # Update a document that has the title "Random Harvest" to have a plot field with the specified value
        result = movies.update_one(
          { "title": "Random Harvest" },
          {
            "$set": {
              "plot": "A harvest of random numbers, such as: {}".format(random.random()),
            },
          },
          # Set the upsert option to insert a document if no documents match the filter
          upsert=True
        )

        # Print the number of matching and modified documents
        print(
          "{} document(s) matched the filter, updated {} document(s)".format(result.matched_count, result.modified_count)
        )
    except Exception as e:
        print(e)
    finally:
        # Close the connection after the operation completes
        client.close()

# Run the program and print any thrown errors
run()