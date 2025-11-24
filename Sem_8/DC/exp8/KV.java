import java.io.Serializable;

public class KV implements Serializable {
    public final String key;
    public final String value;

    public KV(String k, String v) {
        this.key = k;
        this.value = v;
    }

    public String toString() {
        return key + "\t" + value;
    }
}
