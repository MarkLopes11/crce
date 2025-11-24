import java.util.List;

public interface Reducer {
    // key and list of values for that key
    void reduce(String key, List<String> values, Collector collector);
}
