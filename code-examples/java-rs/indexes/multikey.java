import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;

import static com.mongodb.client.model.Filters.eq;
import static com.mongodb.client.model.Projections.fields;
import static com.mongodb.client.model.Projections.include;
import static com.mongodb.client.model.Projections.excludeId;

public class MongoDBAccess {
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> movies = database.getCollection("movies");

        movies.createIndex(new Document("cast", 1)).subscribe(indexName -> {
            System.out.println("Index created: " + indexName);

            Document query = eq("cast", "Viola Davis");
            Document projection = fields(include("cast", "title"), excludeId());

            movies.find(query)
                  .projection(projection)
                  .subscribe(movie -> System.out.println(movie.toJson()), 
                             error -> error.printStackTrace(), 
                             mongoClient::close);
        });
    }
}
