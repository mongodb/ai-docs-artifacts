import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.client.result.UpdateResult;
import org.bson.Document;
import java.util.Arrays;

public class Main {
    public static void main(String[] args) {
        ConnectionString connectionString = new ConnectionString("<connection string uri>");
        MongoClient client = MongoClients.create(connectionString);
        MongoDatabase database = client.getDatabase("testdb");
        MongoCollection<Document> inventoryCollection = database.getCollection("inventory");
        MongoCollection<Document> ordersCollection = database.getCollection("orders");

        Document[] items = new Document[] {
                new Document("item", "sunblock").append("qty", 85).append("price", 6.0),
                new Document("item", "beach chair").append("qty", 30).append("price", 25.0)
        };
        inventoryCollection.insertMany(Arrays.asList(items)).subscribe();

        Document[] order1 = new Document[] {
                new Document("item", "sunblock").append("qty", 3),
                new Document("item", "beach chair").append("qty", 1)
        };

        Document[] order2 = new Document[] {
                new Document("item", "volleyball").append("qty", 1)
        };

        Document[] order = order1;

        client.startSession().subscribe(session -> {
            session.startTransaction();

            double total = 0;
            for (Document item : order) {
                inventoryCollection.findOneAndUpdate(session,
                        new Document("item", item.getString("item")).append("qty", new Document("$gte", item.getInteger("qty"))),
                        new Document("$inc", new Document("qty", -item.getInteger("qty")))).subscribe(inStock -> {
                    if (inStock == null) {
                        session.abortTransaction();
                        System.out.println("Item not found or insufficient quantity.");
                        return;
                    }
                    double subTotal = item.getInteger("qty") * inStock.getDouble("price");
                    total += subTotal;
                });
            }

            Document receipt = new Document("date", new Date())
                    .append("items", Arrays.asList(order))
                    .append("total", total);

            ordersCollection.insertOne(session, receipt).subscribe(success -> {
                session.commitTransaction();
                System.out.println("Order successfully completed and recorded!\nReceipt:\n" + receipt.toJson());
                client.close();
            }, err -> {
                session.abortTransaction();
                System.out.println(err.getMessage());
                client.close();
            });
        });
    }
}
