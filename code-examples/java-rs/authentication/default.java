import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.Success;

import org.bson.Document;

import java.util.concurrent.TimeUnit;

public class MongoConnect {
    public static void main(String[] args) {

        // Replace the following with values for your environment.
        String username = java.net.URLEncoder.encode("<username>", "UTF-8");
        String password = java.net.URLEncoder.encode("<password>", "UTF-8");
        String clusterUrl = "<MongoDB cluster url>";

        String authMechanism = "DEFAULT";

        // Replace the following with your MongoDB deployment's connection string.
        String uri =
                "mongodb+srv://" + username + ":" + password + "@" + clusterUrl + "/?authMechanism=" + authMechanism;

        // Create a new MongoClient
        MongoClient mongoClient = MongoClients.create(uri);

        try {
            // Establish and verify connection
            mongoClient.getDatabase("admin").runCommand(new Document("ping", 1))
                    .toPublisher()
                    .subscribe(success -> System.out.println("Connected successfully to server"),
                            throwable -> System.out.println(throwable.getMessage()));

            // sleep for a while to allow connection to get established and query to get executed before the close operation.
            TimeUnit.SECONDS.sleep(5);
        } finally {
            // Ensures that the client will close when you finish/error
            mongoClient.close();
        }
    }
}
