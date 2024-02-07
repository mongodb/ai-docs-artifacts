// Import the necessary libraries
import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.MongoClientSettings;
import com.mongodb.ServerApi;
import com.mongodb.ServerApiVersion;

public class ConnectToMongoDB {

    // Replace the placeholders in the connection string uri with your credentials
    String uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";
    
    ConnectionString connString = new ConnectionString(uri);

    // Options for the MongoDB client
    MongoClientSettings settings = MongoClientSettings.builder()
        .applyConnectionString(connString)
        .serverApi(ServerApi.builder().version(ServerApiVersion.V1).build())
        .build();

    // Create MongoDB client
    com.mongodb.reactivestreams.client.MongoClient mongoClient = MongoClients.create(settings);
}
