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
     * Fields to permit Peterson's algorithm
     * flag: -1, 0, ..., 8
     * turn: 0, 1, ..., 9
     */
    private static int[10] flag;
    private static int[10] turn;
    private static int procCount;

    /**
     * The processor number
     */
    private int pNo;

    public Processor (int pNo) {

    }

    public void EntrySection () {
        for (int k = 0; k <= Processor.procCount - 2) {
            Processor.flag[pNo] = k;
            Processor.turn[k] = pNo;
            while (E(j != i, Processor.flag[j] >= k && Processor.turn[k] == i)) {
                nop();
            }
        }
    }

    public void ExitSection () {
        Processor.flag[pNo] = -1;
    }

    public int main () {
        // Peterson's n-process algorithm for n = 10
        flag.fill(-1);
        turn.fill(0);
    }
}
