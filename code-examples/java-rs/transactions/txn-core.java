/* Here we use the MongoDB Java Driver Library for making write operations in a transaction */

import com.mongodb.ConnectionString;
import com.mongodb.ClientSessionOptions;
import com.mongodb.client.*;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Updates;
import com.mongodb.reactivestreams.client.*;
import org.bson.Document;
import com.mongodb.TransactionOptions;

import static com.mongodb.client.model.Filters.eq;

public class WriteTransaction {
    
    private static final ConnectionString MONGO_CONNECTION_STRING = new ConnectionString(System.getenv("MONGODB_URI"));
    private static final MongoClient mongoClient = MongoClients.create(MONGO_CONNECTION_STRING);
    
    public static void cleanUp(MongoClient mongoClient) {
        MongoDatabase database = mongoClient.getDatabase("testdb");
        MongoIterable<String> collection = database.listCollectionNames();
        for (String name : collection) {
            if (name.equals("customers") || name.equals("orders") || name.equals("inventory")) {
                database.getCollection(name).drop();
            }
        }
    }

    public static void setup(MongoClient mongoClient) {
        MongoDatabase database = mongoClient.getDatabase("testdb");
        MongoCollection<Document> customerCollection = database.getCollection("customers");
        MongoCollection<Document> inventoryCollection = database.getCollection("inventory");
        
        Document customer = new Document("_id", 98765)
                            .append("orders", new ArrayList<>());
        customerCollection.insertOne(customer);
        
        List<Document> items = Arrays.asList(
            new Document("item", "sunblock").append("item_id", 5432).append("qty", 85),
            new Document("item", "beach towel").append("item_id", 7865).append("qty", 41)
        );
        inventoryCollection.insertMany(items);
    }

    public static void queryData() {
        MongoDatabase database = mongoClient.getDatabase("testdb");
        database.listCollectionNames().map(collectionName -> database.getCollection(collectionName).aggregate(Lists.newArrayList(
            Aggregates.group(null, null)
        )).to(toList()).subscribe(System.out::println));
    }

    public static void placeOrder(MongoClient mongoClient, List<Document> cart, Document payment) {
        MongoDatabase database = mongoClient.getDatabase("testdb");
        MongoCollection<Document> ordersCollection = database.getCollection("orders");
        MongoCollection<Document> inventoryCollection = database.getCollection("inventory");
        MongoCollection<Document> customerCollection = database.getCollection("customers");

        ClientSession clientSession = mongoClient.startSession(ClientSessionOptions.builder().causalConsistency(true).build());
        clientSession.startTransaction(TransactionOptions.builder().writeConcern(WriteConcern.MAJORITY).build());

        Document order = new Document("customer", payment.get("customer"))
                        .append("items", cart)
                        .append("total", payment.get("total"));
        ordersCollection.insertOne(clientSession, order);

        for (Document item : cart) {
            FindOneAndUpdateOptions options = new FindOneAndUpdateOptions().returnDocument(ReturnDocument.AFTER);
            UpdateResult updateResult = inventoryCollection.updateOne(eq("item_id", item.get("item_id")),
                    Updates.inc("qty", -((Integer) item.get("qty"))), options);
            if (updateResult.getMatchedCount() == 0) {
                clientSession.abortTransaction();
                throw new IllegalArgumentException("Insufficient quantity or item ID not found");
            }
        }

        UpdateResult updateResult = customerCollection.updateOne(clientSession,
                eq("_id", payment.get("customer")),
                Updates.push("orders", order.get("_id")));
        clientSession.commitTransaction();
        clientSession.close();
    }

    public static void run() {
        cleanUp(mongoClient);
        setup(mongoClient);

        Document order1 = new Document("item", "sunblock")
                        .append("item_id", 5432)
                        .append("qty", 1)
                        .append("price", 5.19);

        Document order2 = new Document("item", "beach towel")
                        .append("item_id", 7865)
                        .append("qty", 2)
                        .append("price", 15.99);

        List<Document> cart = Arrays.asList(order1, order2);
        Document payment = new Document("customer", 98765).append("total", 37.17);

        placeOrder(mongoClient, cart, payment);
        cleanUp(mongoClient);
        mongoClient.close();
        queryData();
    }

    public static void main(String[] args) {
        run();
    }
}
