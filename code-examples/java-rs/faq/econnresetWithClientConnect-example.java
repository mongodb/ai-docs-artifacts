import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;

String uri = "mongodb://localhost:27017/test?maxPoolSize=5000";
// create a new MongoClient
MongoClient client = MongoClients.create(uri);

// The connection is started when the Server is started or upon first usage.
