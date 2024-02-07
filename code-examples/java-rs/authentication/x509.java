import com.mongodb.ConnectionString;
import com.mongodb.reactivestreams.client.MongoClients;
import com.mongodb.reactivestreams.client.MongoClient;
import com.mongodb.MongoClientSettings;
import com.mongodb.reactivestreams.client.MongoDatabase;
import com.mongodb.ServerAddress;
import com.mongodb.connection.ClusterSettings;
import com.mongodb.connection.SslSettings;
import org.bson.Document;
import com.mongodb.async.SingleResultCallback;
import io.reactivex.Observable;
import io.reactivex.Observer;
import io.reactivex.disposables.Disposable;

import java.util.Arrays;
import java.util.concurrent.TimeUnit;

public class ConnectToDB {
    private static final String clusterUrl = "<MongoDB cluster url>";
    private static final String clientPEMFile = "<path to the client PEM certificate file>";

    public static void main(String[] args) {
        ConnectionString connectionString = new ConnectionString(
                "mongodb+srv://" + clusterUrl + "/?authSource=$external&ssl=true&sslClientCertificateKeyFile="+ clientPEMFile);
        ClusterSettings clusterSettings = ClusterSettings.builder().applyConnectionString(connectionString).build();
        SslSettings sslSettings = SslSettings.builder().applyConnectionString(connectionString).build();
        MongoClientSettings settings = MongoClientSettings.builder()
                .applyConnectionString(connectionString)
                .clusterSettings(clusterSettings)
                .sslSettings(sslSettings)
                .build();

        MongoClient mongoClient = MongoClients.create(settings);
        
        MongoDatabase database = mongoClient.getDatabase("admin");
        
        Observable<Document> observable = com.mongodb.reactivestreams.client.ObservableToPublisher
                .publisherToObservable(database.runCommand(new Document("ping", 1)));

        observable
                .subscribe(new Observer<Document>() {
                    @Override
                    public void onSubscribe(Disposable d) {
                    }

                    @Override
                    public void onNext(Document document) {
                        System.out.println("Connected successfully to server");
                    }

                    @Override
                    public void onError(Throwable e) {
                        e.printStackTrace();
                    }

                    @Override
                    public void onComplete() {
                        mongoClient.close();
                    }
                });
    }
}
