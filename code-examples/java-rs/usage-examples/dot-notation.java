import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import org.bson.Document;

MongoDatabase database = MongoClients.create("<your db>").getDatabase("<your db>");
MongoCollection<Document> myColl = database.getCollection("<your collection>");

Document updateQuery = new Document();
updateQuery.put("field.nested", "A string");
myColl.updateOne(new Document(), new Document("$set", updateQuery));

updateQuery = new Document();
Document nestedDocument = new Document();
nestedDocument.put("nested", "A string");
updateQuery.put("field", nestedDocument);
myColl.updateOne(new Document(), new Document("$set", updateQuery));

MongoDatabase database = MongoClients.create("<your database>").getDatabase("<your database>");
MongoCollection<Document> myColl = database.getCollection("<your collection>");
myColl.find(new Document("age", "Accepts any type!"));
