import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.client.result.UpdateResult;
import com.mongodb.reactivestreams.client.MongoCollection;

import org.bson.Document;

import java.util.Random;
import java.util.regex.Pattern;

public class Main {

    private static final MongoClient client = MongoClients.create("<connection string uri>");
    private static final MongoDatabase database = client.getDatabase("sample_mflix");
    private static final MongoCollection<Document> movies = database.getCollection("movies");

    public static void main(String[] args) {
        try {
            run().subscribe(
                updateResult -> System.out.println(String.format("Modified %d document(s)", updateResult.getModifiedCount())),
                Throwable::printStackTrace,
                () -> client.close()
            );
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private static Mono<UpdateResult> run() {
        Pattern pattern = Pattern.compile("The Cat from");

        return Mono.from(movies.replaceOne(
            new Document("title", pattern),
            new Document("title", "The Cat from Sector " + (new Random().nextInt(1000) + 1))
        ));
    }
}
