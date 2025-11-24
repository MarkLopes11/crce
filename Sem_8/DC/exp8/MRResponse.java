import java.io.Serializable;
import java.util.Map;

public class MRResponse implements Serializable {
    public final boolean ok;
    public final String error;
    public final Map<String, Object> data;

    public MRResponse(boolean ok, String err, Map<String, Object> data) {
        this.ok = ok;
        this.error = err;
        this.data = data;
    }

    public static MRResponse ok(Map<String, Object> d) {
        return new MRResponse(true, null, d);
    }

    public static MRResponse fail(String err) {
        return new MRResponse(false, err, null);
    }
}
