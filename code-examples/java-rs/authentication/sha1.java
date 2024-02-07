import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import org.bson.Document;
import java.util.Arrays;

public class Main {
    private MongoClient mongoClient;

    public static void main(String[] args) {
        new Main().connectToDatabase();
    }

    private void connectToDatabase() {
        char[] password = "<password>".toCharArray(); 
        String username = "<username>";
        String clusterUrl = "<MongoDB cluster url>";

        ConnectionString connectionString = new ConnectionString(
                "mongodb+srv://" + username + ":" + new String(password) + "@" + clusterUrl + "/?authSource=admin&authMechanism=SCRAM-SHA-1"
        );

        mongoClient = MongoClients.create(connectionString);

        mongoClient.getDatabase("admin").runCommand(new Document("ping", 1))
                .subscribe(success -> System.out.println("Connected successfully to server"),
                        error -> System.out.println("An error occurred: " + error.getMessage()),
                        this::disconnectFromDatabase);
    }

    private void disconnectFromDatabase() {
        mongoClient.close();
    }
}
