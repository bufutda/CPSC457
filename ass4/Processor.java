import java.util.concurrent.*;

/**
 * This class represents a processor. Each processor is executed in a seperate thread.
 */
public class Processor {
    /**
     * WriteBuffer, shared with the processor's MemoryAgent
     */
    private WriteBuffer writeBuffer;

    /**
     * MainMemory, shared with all processors and MemoryAgents
     */
    private MainMemory mem;

    /**
     * MemoryAgent, owned by the processor
     */
    private MemoryAgent memAgent;

    /**
     * Implementation of Peterson's n-process algorithm (in this case n = 10)
     */
    public void petersonCritical () {

    }
}
