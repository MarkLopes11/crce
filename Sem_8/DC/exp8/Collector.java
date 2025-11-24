import java.util.List;
import java.util.ArrayList;

public class Collector {
    private final List<KV> list = new ArrayList<>();

    public void emit(String k, String v) {
        list.add(new KV(k, v));
    }

    public List<KV> get() {
        return list;
    }
}
