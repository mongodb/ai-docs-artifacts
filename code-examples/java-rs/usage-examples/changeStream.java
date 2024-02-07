// Required MongoDB Java Reactive Streams Driver dependencies
import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.client.model.changestream.ChangeStreamDocument;
import org.bson.Document;
import reactor.core.publisher.Flux;

public class WatchCollectionChanges {

    // Replace the uri string with your MongoDB deployment's connection string
    private static final String CONNECTION_STRING = "<connection string uri>";

    public static void main(final String[] args) {
        MongoClient mongoClient = MongoClients.create(new ConnectionString(CONNECTION_STRING));

        try {
            MongoDatabase database = mongoClient.getDatabase("insertDB");
            MongoCollection<Document> haikus = database.getCollection("haikus");

            // Create a flux i.e watch change stream on the "haikus" collection
            Flux<ChangeStreamDocument<Document>> changeStreamFlux = Flux.from(haikus.watch());

            // Print change events as they occur
            changeStreamFlux.doOnNext(change -> System.out.println("Received change: " + change)).blockLast();

        } finally {
            // Close the MongoDB client connection
            mongoClient.close();
        }
    }
}
