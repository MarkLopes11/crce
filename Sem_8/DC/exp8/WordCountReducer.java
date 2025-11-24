import java.util.List;

public class WordCountReducer implements Reducer {

    public void reduce(String key, List<String> values, Collector collector) {
        int sum = 0;
        for (String v : values) {
            sum += Integer.parseInt(v);
        }
        collector.emit(key, Integer.toString(sum));
    }
}
