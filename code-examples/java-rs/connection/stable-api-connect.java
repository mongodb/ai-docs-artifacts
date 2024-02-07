import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.ConnectionString;
import com.mongodb.MongoClientSettings;
import com.mongodb.ServerApiVersion;
import com.mongodb.ServerApi;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import com.mongodb.client.result.UpdateResult;
import reactor.core.publisher.Mono;

public class Main {
    public static void main(String[] args) {
        // Replace the placeholder with your Atlas connection string
        ConnectionString connectionString = new ConnectionString("<connection string>");

        MongoClientSettings settings = MongoClientSettings.builder()
                .applyConnectionString(connectionString)
                .serverApi(new ServerApi.Builder()
                        .version(ServerApiVersion.V1)
                        .strict(true)
                        .deprecationErrors(true)
                        .build())
                .build();
        // Create a MongoClient with a MongoClientMetrics object to set the Stable API version
        MongoClient mongoClient = MongoClients.create(settings);

        try {
            MongoDatabase database = mongoClient.getDatabase("admin");

            Mono.from(database.runCommand(new Document("ping", 1)))
                    .doOnSuccess(doc -> System.out.println("Pinged your deployment. You successfully connected to MongoDB!"))
                    .doOnError(error -> System.out.println("An error occurred: " + error))
                    .block();
        } finally {
            mongoClient.close();
        }
    }
}
