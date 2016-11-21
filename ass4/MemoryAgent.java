import java.util.concurrent.*;

/**
 * The processor's agent that communicates to the write-buffer's pending STOREs with MainMemory. A memory agent executes in a spereate thread. Each agent shares a property (object) with other agents of type MainMemory. It also shares a WriteBuffer object with its processor. A MemoryAgent is responsible for flushing a pending store in the write-buffer to memory (the store is removed from the buffer and committed to memory)
 */
public class MemoryAgent {
    /**
     * A MainMemory object, to be shared with all Agents and Processors
     */
    private MainMemory mem;

    /**
     * A WriteBuffer object, to be shared with the linked Processor
     */
    private WriteBuffer writeBuffer;

    /**
     * Flushes the entire WriteBuffer
     */
    public void flush () {

    }

    /**
     * Flushes a single STORE in the writeBuffer (TSO)
     */
    public void flushSingleStore () {

    }

    /**
     * Flushes a single STORE of variable x in the writeBuffer (PSO)
     * @param x    the variable to store
     */
    public void flushSingleStore (int x) {

    }
}
