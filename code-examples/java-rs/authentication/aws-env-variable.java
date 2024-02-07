import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoDatabase;

import org.bson.Document;

import java.util.concurrent.CompletionStage;

public class MongoDBClient {
    
    private static final String CLUSTER_URL = "<MongoDB deployment url>";
    private static final String AUTH_MECHANISM = "MONGODB-AWS";
    
    public static void main(String[] args) {
        String uri = "mongodb+srv://" + CLUSTER_URL + "/?authSource=%24external&authMechanism=" + AUTH_MECHANISM;
        
        ConnectionString connectionString = new ConnectionString(uri);
        MongoClient client = MongoClients.create(connectionString);

        run(client).whenComplete((success, throwable) -> client.close());
    }
    
    public static CompletionStage<Void> run(MongoClient client) {
        MongoDatabase adminDb = client.getDatabase("admin");
        
        return adminDb.runCommand(new Document("ping", 1))
                .toCompletionStage()
                .thenAccept(success -> System.out.println("Connected successfully to server."))
                .exceptionally(throwable -> {
                    throwable.printStackTrace();
                    return null;
                });
    }
}
