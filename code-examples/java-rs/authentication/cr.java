import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;

import org.bson.Document;

import java.util.concurrent.TimeUnit;

public class MongoDBConnection {
    private static final String username = java.net.URLEncoder.encode("<username>", "UTF-8");
    private static final String password = java.net.URLEncoder.encode("<password>", "UTF-8");
    private static final String clusterUrl = "<MongoDB cluster url>";

    private static final String uri =
        "mongodb+srv://" + username + ":" + password + "@" + clusterUrl + "/?authMechanism=" + authMechanism + "&tls=true&tlsCertificateKeyFile=" + clientPEMFile;

    private static MongoClient mongoClient;

    public static void main(String[] args) {
        mongoClient = MongoClients.create(new ConnectionString(uri));

        MongoDatabase database = mongoClient.getDatabase("admin");

        database.runCommand(new Document("ping", 1))
                .toCompletableFuture()
                .thenAccept(document -> System.out.println("Connected successfully to server"))
                .exceptionally(throwable -> {
                    throwable.printStackTrace();
                    return null;
                })
                .thenRun(() -> {
                    mongoClient.close();
                    try {
                        Thread.sleep(2000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                });
    }
}
