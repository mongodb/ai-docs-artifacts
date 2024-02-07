import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.*;
import org.bson.Document;

import java.util.Arrays;
import java.util.concurrent.TimeUnit;

import static com.mongodb.reactivestreams.client.MongoClients.create;
import static com.mongodb.client.model.Filters.*;
import static com.mongodb.client.model.Projections.*;
import static com.mongodb.client.model.Sorts.descending;

public class MainClass {
  private static final String connectionString = "mongodb+srv://<user>:<password>@<cluster-url>";
  
  public static void main(String[] args) {
    final MongoClient mongoClient = create(new ConnectionString(connectionString));
    final MongoDatabase database = mongoClient.getDatabase("sample_mflix");
    final MongoCollection<Document> movies = database.getCollection("movies");

    word(movies);
    phrase(movies);
    negation(movies);
    relevance(movies);

    mongoClient.close();
  }

  private static void word(MongoCollection<Document> movies) {
    Document query = new Document("$text", new Document("$search", "trek"));
    movies.find(query).projection(fields(include("title"), excludeId())).forEach(printBlock,System.err::println);
  }

  private static void phrase(MongoCollection<Document> movies) {
    Document query = new Document("$text", new Document("$search", "\"star trek\""));
    movies.find(query).projection(fields(include("title"), excludeId())).forEach(printBlock,System.err::println);
  }

  private static void negation(MongoCollection<Document> movies) {
    Document query = new Document("$text", new Document("$search", "\"star trek\" -\"into darkness\""));
    movies.find(query).projection(fields(include("title"), excludeId())).forEach(printBlock,System.err::println);
  }

  private static void relevance(MongoCollection<Document> movies) {
    Document query = new Document("$text", new Document("$search", "\"star trek\" -\"into darkness\""));
    movies.find(query).sort(descending("score")).projection(fields(include("title", "score"), excludeId())).forEach(printBlock,System.err::println);
  }

  private static final Consumer<Document> printBlock = new Consumer<Document>() {
    @Override
    public void accept(Document document) {
      System.out.println(document.toJson());
    }
  };
}
