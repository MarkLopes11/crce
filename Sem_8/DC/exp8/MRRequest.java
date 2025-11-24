import java.io.Serializable;
import java.util.Map;

public class MRRequest implements Serializable {

    public enum Type {
        REGISTER,
        MAP_TASK,
        REDUCE_TASK,
        HEARTBEAT,
        SUBMIT
    }

    public final Type type;
    public final Map<String, Object> data;

    public MRRequest(Type t, Map<String, Object> d) {
        this.type = t;
        this.data = d;
    }
}
