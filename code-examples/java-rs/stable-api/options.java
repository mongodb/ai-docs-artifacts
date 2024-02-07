// Import the necessary MongoDB classes
import com.mongodb.ConnectionString;
import com.mongodb.MongoClientSettings;
import com.mongodb.ServerApi;
import com.mongodb.ServerApiVersion;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;

// Replace the placeholders in the connection string uri with your credentials
String uri = "mongodb+srv://<user>:<password>@<cluster-url>?retryWrites=true&w=majority";

//Create ServerApi settings for version 1, strict mode, and deprecation errors
ServerApi serverApi = ServerApi.builder()
   .version(ServerApiVersion.V1)
   .strict(true)
   .deprecationErrors(true)
   .build();

// Create a Mongo client settings with the uri and serverApi settings
MongoClientSettings settings = MongoClientSettings.builder()
    .applyConnectionString(new ConnectionString(uri))
    .serverApi(serverApi)
    .build();

// Create a client using the settings
MongoClient client = MongoClients.create(settings);
