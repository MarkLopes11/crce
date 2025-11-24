import java.io.Serializable;
import java.util.List;

public class ChunkInfo implements Serializable {

    public String chunkId;
    public List<String> locations; // host:port strings

    public ChunkInfo(String chunkId, List<String> locations) {
        this.chunkId = chunkId;
        this.locations = locations;
    }
}
