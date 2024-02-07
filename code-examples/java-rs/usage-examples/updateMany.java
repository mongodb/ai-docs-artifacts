import com.mongodb.MongoClient;
import com.mongodb.reactiverestreams.client.MongoDatabase;
import com.mongodb.reactiverestreams.client.MongoCollection;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Updates;
import org.bson.Document;
import java.util.concurrent.TimeUnit;
import java.util.Random;

public class MovieUpdateSample {
  enum Rating {
    G, PG, PG_13, R, NR;
  }

  static class Movie {
    Rating rated;
    String randomReview;
    
    // getters and setters
  }

  public static void main(String[] args) {
    MongoClient mongoClient = new MongoClient("<connection string uri>");
    MongoDatabase database = mongoClient.getDatabase("sample_mflix");
    MongoCollection<Document> movies = database.getCollection("movies");

    try {
      movies.updateMany(
        Filters.eq("rated", Rating.G.name()),
        Updates.set("random_review", "After viewing I am " + new Random().nextFloat() * 100 + "% more satisfied with life.")
      ).subscribe(updateResult -> {
            System.out.println("Updated " + updateResult.getModifiedCount() + " documents");
        }, error -> {
            System.err.println("An error occurred.");
            error.printStackTrace();
        }, () -> {
            // Close connection in onComplete of operation.
            mongoClient.close();
        });
    } catch (Exception e) {
      e.printStackTrace();
    } finally {
      // Close connection on completion or error after a timeout.
      mongoClient.close(10, TimeUnit.SECONDS);
    }
  }
}
