import java.io.Serializable;
import java.util.Map;

public class Request implements Serializable {

    public enum Type {
        REGISTER_CHUNKSERVER,
        HEARTBEAT,
        CREATE_FILE,
        GET_FILE_INFO,
        STORE_CHUNK,
        FETCH_CHUNK,
        DELETE_CHUNK,
        SHUTDOWN
    }

    public Type type;
    public Map<String, Object> data;

    public Request(Type type, Map<String, Object> data) {
        this.type = type;
        this.data = data;
    }
}
