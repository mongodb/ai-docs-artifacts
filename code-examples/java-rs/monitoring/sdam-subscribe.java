import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.BsonDocument;
import com.mongodb.MongoCommandException;

import java.util.concurrent.CountDownLatch;

public class SDAMEventSubscription {
    // Replace the following with your MongoDB deployment's connection string
    private static final String CONNECTION_STRING = "mongodb+srv://<clusterUrl>/?replicaSet=rs&writeConcern=majority";
    MongoClient mongoClient;

    // Replace <event name> with the name of the event you are subscribing to
    private static final String EVENT_NAME = "<event name>";

    public SDAMEventSubscription() {
        mongoClient = MongoClients.create(new ConnectionString(CONNECTION_STRING));
        mongoClient.getClusterDescription().getChangedEvent().subscribe(event -> {
            if (event.getName().equals(EVENT_NAME)) {
                System.out.println(String.format("Received %s: %s", EVENT_NAME, event.toJson()));
            }
        });
    }

    public void run() {
        MongoDatabase database = mongoClient.getDatabase("admin");
        CountDownLatch latch = new CountDownLatch(1);
        try {
            database.runCommand(BsonDocument.parse("{ ping: 1 }"))
                    .subscribe(result -> {
                        latch.countDown();
                        System.out.println("Connected successfully");
                    });
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            // Close the database connection on completion or error
            mongoClient.close();
        }
    }

    public static void main(String[] args) {
        SDAMEventSubscription sdamEventSubscription = new SDAMEventSubscription();
        try {
            sdamEventSubscription.run();
        } catch (MongoCommandException e) {
            e.printStackTrace();
        }
    }
}
