public interface Mapper {
    // key: split id or line number; value: input text
    void map(String key, String value, Collector collector);
}
