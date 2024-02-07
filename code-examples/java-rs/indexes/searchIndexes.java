import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import org.bson.Document;
import org.bson.conversions.Bson;
import com.mongodb.client.model.Indexes;

import static com.mongodb.client.model.Filters.eq;

public class SearchIndexOps {
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("mongodb://<user>:<password>@<cluster_url>");

        MongoDatabase database = mongoClient.getDatabase("<databaseName>");
        MongoCollection<Document> collection = database.getCollection("<collectionName>");

        Document index1 = new Document()
                .append("name", "search1")
                .append("definition", new Document("mappings",
                        new Document("dynamic", true)));

        collection.createIndex(index1).subscribe(success -> {
            // List Search Indexes
            collection.listIndexes().subscribe(doc -> {
                System.out.println(doc.toJson());
            }, throwable -> {
                System.out.println("Failed to list indexes");
            });

            // Update Search Index
            Bson updateIndex = Indexes.text("search1");
            collection.createIndex(updateIndex).subscribe(updatedIndex -> {
                System.out.println("Updated Index: " + updatedIndex);

                // Drop Search Index
                collection.dropIndex("search1").subscribe(success2 -> {
                    System.out.println("Dropped Index: " + success2);
                }, throwable -> {
                    System.out.println("Failed to drop index");
                });

            }, throwable -> {
                System.out.println("Failed to update index");
            });

        }, throwable -> {
            System.out.println("Failed to create index");
        });

        mongoClient.close();
    }
}
