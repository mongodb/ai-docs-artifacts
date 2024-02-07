import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import org.bson.Document;
import com.mongodb.client.model.Filters;
import java.util.NoSuchElementException;

public class Main {

    public static void main(String[] args) {
        
        MongoClient mongoClient = MongoClients.create("<connection string uri>");
        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> movies = database.getCollection("movies");

        movies.distinct("year", Filters.eq("directors", "Barbra Streisand"), Integer.class)
            .subscribe(
                year -> System.out.println(year),
                throwable -> { throw new NoSuchElementException("No such element exists"); },
                () -> mongoClient.close()
            );
    }
}
