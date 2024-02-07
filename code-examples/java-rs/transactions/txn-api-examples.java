import com.mongodb.reactivestreams.client.*;
import com.mongodb.TransactionOptions;
import com.mongodb.ReadPreference;
import com.mongodb.WriteConcern;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Updates;
import org.bson.Document;
import org.reactivestreams.Publisher;
import reactor.core.publisher.Mono;

import java.util.function.Supplier;

public class TransferTransaction {

    private static MongoClient mongoClient = MongoClients.create("<connection string>");
    private static MongoDatabase bankDb = mongoClient.getDatabase("bank");

    public static void main(String[] args) {

        Mono.from(coreTest())
            .doOnError(t -> System.out.println("An error occurred during the transaction:" + t.getMessage()))
            .then(Mono.from(convTest()))
            .doOnSuccess(val -> System.out.println(val))
            .subscribe(val -> {}, t -> t.printStackTrace(), () -> mongoClient.close());
    }

    private static Publisher<?> coreTest() {
        ClientSession clientSession = mongoClient.startSession();

        return Mono.fromSubscriber(clientSession.startTransaction())
            .then(processTransaction(clientSession))
            .doOnSuccess(success -> Mono.from(clientSession.commitTransaction()))
            .doOnError(failure -> Mono.from(clientSession.abortTransaction()))
            .doFinally(signalType -> Mono.from(clientSession.close()));
    }

    private static Publisher<?> convTest() {
        TransactionOptions txnOptions = TransactionOptions.builder()
            .readPreference(ReadPreference.primary())
            .writeConcern(WriteConcern.MAJORITY)
            .build();

        return Mono.from(
            mongoClient.startSession()
                .withTransaction(() -> processTransaction(mongoClient.startSession()), txnOptions))
            .doFinally(signalType -> mongoClient.close())
            .thenReturn("Transaction committed.");
    }

    private static Mono<Void> processTransaction(ClientSession clientSession) {
        MongoCollection<Document> savingsColl = bankDb.getCollection("savings_accounts");
        MongoCollection<Document> checkingColl = bankDb.getCollection("checking_accounts");

        return Mono.when(
            Mono.from(savingsColl.findOneAndUpdate(clientSession, 
                Filters.eq("account_id", "9876"), 
                Updates.inc("amount", -100))),
            Mono.from(checkingColl.findOneAndUpdate(clientSession, 
                Filters.eq("account_id", "9876"), 
                Updates.inc("amount", 100)))
        );
    }
}
