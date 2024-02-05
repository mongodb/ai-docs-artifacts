# Execute a find command
try:
    collection.find({"$where": "sleep(100) || true"}).max_time_ms(50)
except pymongo.errors.ExecutionTimeout:
    pass