// Create a new Bson filter to match documents where "type" field is "photograph"
Bson filter = Filters.eq("type", "photograph");

// Asynchronously find documents using the filter
myColl.find(filter).subscribe(new Observer<Document>() {
    
    @Override
    public void onNext(Document document) {
        System.out.println(document.toJson()); // Handle each matched document
    }

    @Override
    public void onError(Throwable throwable) {
        // Handle any error that might occur while processing the result
        throwable.printStackTrace();
    }

    @Override
    public void onComplete() {
        // Complete handling the result
        System.out.println("Operation completed.");
    }
});
