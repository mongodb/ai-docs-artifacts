# Delete multiple documents

import pymongo

# Replace the uri string with your MongoDB deployment's connection string
uri = "<connection string uri>"

client = pymongo.MongoClient(uri)


def run():
    try:
        # Access the 'sample_mflix' database and 'movies' collection
        database = client['sample_mflix']
        movies = database['movies']

        # Delete all documents that match the specified regular
        # expression in the title field from the 'movies' collection
        result = movies.delete_many({'title': {'$regex': 'Santa'}})

        # Print the number of deleted documents
        print(f'Deleted {result.deleted_count} documents')
    except Exception as e:
        print(e)
    finally:
        # Close the connection after the operation completes
        client.close()


# Run the program
run()