import java.util.concurrent.*;

/**
 * Represents a WriteBuffer.
 */
public class WriteBuffer {
    /**
     * If tso is true, then the write-buffer is FIFO, if not then it is only FIFO per variable (PSO).
     */
    private boolean tso;

    /**
     * Returns the value of x from the buffer.
     * @param x    the key of the pair to return from the buffer
     * @throws NotInBufferException if x is not in the buffer
     * @returns the value of x
     */
    public byte load (int x) throws NotInBufferException {

    }

    /**
     * Sends a write to x of value v to the buffer
     * @param x    the key of the pair to write to
     * @param v    the value to write
     */
    public void store (int x, byte v) {

    }

    /**
     * Atomically swaps x and v without interruption
     * @param x    an address in main memory or buffer
     * @param v    a new value for *x
     * @returns the old value of x
     */
    public byte SwapAtomic (int x, byte v) {
        // tmp = *x
        // *x = v
        // return tmp
    }
}
