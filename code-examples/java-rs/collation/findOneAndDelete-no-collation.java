MongoCollection<Document> myColl;

myColl.findOneAndDelete(Filters.gt("a", "100")).subscribe(new Observer<DeleteResult>() {
    @Override
    public void onNext(final DeleteResult result) {
        System.out.println("Deleted: " + result.getDeletedCount());
    }

    @Override
    public void onError(final Throwable t) {
        System.out.println("Failed: " + t.getMessage());
    }

    @Override
    public void onComplete() {
        System.out.println("Completed");
    }
});
