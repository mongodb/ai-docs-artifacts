import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.Success;
import org.bson.Document;
import org.reactivestreams.Publisher;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

public class MongoDBAsyncConnection {

    private static final String accessKeyID = "<AWS_ACCESS_KEY_ID>";
    private static final String secretAccessKey = "<AWS_SECRET_ACCESS_KEY>";
    private static final String clusterUrl = "<MongoDB cluster url>";
    private static final String authMechanism = "MONGODB-AWS";

    public static void main(String[] args) {
        String connectionString = String.format(
                "mongodb+srv://%s:%s@%s/?authSource=%%24external&authMechanism=%s",
                accessKeyID, secretAccessKey, clusterUrl, authMechanism
        );

        MongoClient mongoClient = MongoClients.create(new ConnectionString(connectionString));

        MongoDatabase database = mongoClient.getDatabase("admin");

        Publisher<Success> publisher = database.runCommand(new Document("ping", 1));

        try {
            publisher.toFuture().get(20, TimeUnit.SECONDS);
            System.out.println("Connected successfully to server.");
        } catch (InterruptedException | ExecutionException | TimeoutException e) {
            e.printStackTrace();
        } finally {
            mongoClient.close();
        }
    }

}
