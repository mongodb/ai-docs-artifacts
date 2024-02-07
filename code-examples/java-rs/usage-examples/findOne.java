import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import org.bson.conversions.Bson;
import java.util.Date;
import static com.mongodb.client.model.Filters.eq;
import static com.mongodb.client.model.Projections.excludeId;
import static com.mongodb.client.model.Projections.fields;
import static com.mongodb.client.model.Projections.include;
import static com.mongodb.client.model.Sorts.descending;

class IMDb {
    int rating;
    int votes;
    int id;
}

class Movie {
    String title;
    int year;
    Date released;
    String plot;
    String type;
    IMDb imdb;
}

class MovieSummary {
    String title;
    IMDb imdb;
}

public class Example {
    private static final String URI = "<connection string uri>";
    private static MongoClient mongoClient = null;
    private static MongoCollection<Document> movies = null;

    static {
        mongoClient = MongoClients.create(new ConnectionString(URI));
        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        movies = database.getCollection("movies");
    }

    public static void main(String[] args) {
        try {
            Bson filter = eq("title", "The Room");
            Bson projection = fields(include("title", "imdb"), excludeId());
            Bson sort = descending("imdb.rating");

            movies
                .find(filter)
                .projection(projection)
                .sort(sort)
                .first()
                .subscribe(movie -> System.out.println(movie.toJson()));
        } finally {
            if(mongoClient != null) {
                mongoClient.close();
            }
        }
    }
}
