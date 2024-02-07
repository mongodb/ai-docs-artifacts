import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;

import org.bson.Document;

import java.util.concurrent.CountDownLatch;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.logging.Logger;

public class MainClass {

    static final String CONNECTION_URI = "<connection string uri>";
    static Logger logger = Logger.getLogger(MainClass.class.getName());

    public static void main(String[] args) throws Exception {

        MongoClient mongoClient = MongoClients.create(CONNECTION_URI);

        MongoDatabase database = mongoClient.getDatabase("sample_mflix");
        MongoCollection<Document> collection = database.getCollection("movies");

        Document query = new Document("runtime", new Document("$lt", 15));

        Document sort = new Document("title", 1);

        Document proj = new Document("_id", 0).append("title", 1).append("imdb", 1);

        final CountDownLatch latch = new CountDownLatch(1);
        final List<Document> results = new ArrayList<>();

        collection.countDocuments(query)
                .subscribe((result) -> {
                    if (result == 0) {
                        logger.warning("No documents found!");
                    }
                });

        collection.find(query)
                .sort(sort)
                .projection(proj)
                .subscribe(new Observer<Document>() {

                    @Override
                    public void onNext(Document doc) {
                        results.add(doc);
                    }

                    @Override
                    public void onError(Throwable th) {
                        th.printStackTrace();
                        latch.countDown();
                    }

                    @Override
                    public void onComplete() {
                        latch.countDown();
                    }
                });

        latch.await();

        results.stream().forEach((doc) -> {
            System.out.println(doc.toJson());
        });

        mongoClient.close();
    }
}
