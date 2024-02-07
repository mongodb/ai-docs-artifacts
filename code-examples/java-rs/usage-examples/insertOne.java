import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import org.bson.Document;
import org.bson.BsonString;

import java.util.concurrent.CountDownLatch;

public class ConnectToMongoDB {
    // Replace the uri string with your MongoDB deployment's connection string.
    private static final String URI = "<connection string uri>";
    private static final MongoClient mongoClient = MongoClients.create(new ConnectionString(URI));
    private static final MongoDatabase database = mongoClient.getDatabase("insertDB");
    private static final MongoCollection<Document> collection = database.getCollection("haiku");
    private static final Document document = new Document();

    public static void main(String[] args){
        try {
            document.append("title", new BsonString("Record of a Shriveled Datum"))
                    .append("content", new BsonString("No bytes, no problem. Just insert a document, in MongoDB"));
                    
            CountDownLatch latch = new CountDownLatch(1);
            
            collection.insertOne(document).subscribe(success -> {
                System.out.println("A document was inserted with the _id: " + document.getObjectId("_id").toString()));
                latch.countDown();
            }, Throwable::printStackTrace);
          
            latch.await();

        } catch (InterruptedException e) {
            e.printStackTrace();
        } finally {
            mongoClient.close();
        }
    }
}
