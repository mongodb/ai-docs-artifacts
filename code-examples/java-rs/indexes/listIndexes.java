import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.Cursor;
import org.bson.Document;

public class ListIndexes {
    public static void main(String[] args) {
  
        MongoClient mongoClient = MongoClients.create("mongodb+srv://<user>:<password>@<cluster-url>?writeConcern=majority");

        MongoDatabase database = mongoClient.getDatabase("<databaseName>");
        MongoCollection<Document> collection = database.getCollection("<collectionName>");

        try {
            printIndexes(collection);
        } catch (Exception e) {
            System.out.println(e.getMessage());
        } finally {
            mongoClient.close();
        }
    }

    private static async void printIndexes(MongoCollection<Document> collection) throws Exception {
        Cursor<Document> cursor = collection.listIndexes().iterator();
        
        while (await cursor.hasNext()) {
            System.out.println(cursor.next().toJson());
        }
    }
}
