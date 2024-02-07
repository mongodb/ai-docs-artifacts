import com.mongodb.reactivestreams.client.MongoCollection;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.client.model.Collation;
import com.mongodb.client.model.Filters;
import org.bson.Document;

MongoDatabase database; // Assume the database instance is already initialized
MongoCollection<Document> collection = database.getCollection("myColl");

collection.findOneAndDelete(
  Filters.gt("a", 100),
  new FindOneAndDeleteOptions().collation(Collation.builder().locale("en").numericOrdering(true).build())
).subscribe();
