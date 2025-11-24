import java.io.Serializable;
import java.util.Map;

public class Response implements Serializable {

    public boolean ok;
    public String error;
    public Map<String, Object> data;

    public Response(boolean ok, String error, Map<String, Object> data) {
        this.ok = ok;
        this.error = error;
        this.data = data;
    }

    public static Response success(Map<String, Object> data) {
        return new Response(true, null, data);
    }

    public static Response fail(String err) {
        return new Response(false, err, null);
    }
}
