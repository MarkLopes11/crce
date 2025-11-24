public class WordCountMapper implements Mapper {

    public void map(String key, String value, Collector collector) {
        String[] toks = value.split("\\W+");
        for (String t : toks) {
            if (t.length() > 0) {
                collector.emit(t.toLowerCase(), "1");
            }
        }
    }
}
