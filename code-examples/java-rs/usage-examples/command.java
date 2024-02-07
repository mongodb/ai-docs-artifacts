import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;
import java.util.concurrent.ExecutionException;

public class Main {
    public static void main(String[] args) {
        MongoClient mongoClient = MongoClients.create("<connection string uri>");

        try {
            MongoDatabase database = mongoClient.getDatabase("sample_mflix");

            database.runCommand(new Document("dbStats", 1))
                    .subscribe(new PrintDocumentSubscriber<>());
        } catch (Exception e) {
            System.err.println("The dbStats command failed with error: " + e.getMessage());
        } finally {
            try {
                mongoClient.close();
            } catch (Exception e) {
                System.err.println("Failed to close MongoClient with error: " + e.getMessage());
            }
        }
    }
}

class PrintDocumentSubscriber<T> extends OperationSubscriber<Document> {
    @Override
    public void onNext(Document document) {
        System.out.println(document.toJson());
    }

    @Override
    public void onError(Throwable error) {
        System.err.println("The dbStats command failed with error: " + error.getMessage());
    }
    
    @Override
    public void onComplete() {
        System.out.println("Operation completed successfully");
    }
}
