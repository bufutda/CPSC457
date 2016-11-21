/**
 * An exception to be thrown when a load is requested for an object not in a writeBuffer
 */
public class NotInBufferException extends Exception {
    /**
     * UID for object serialization
     */
    private static final long serialVersionUID = -6130314864426490697L;

    /**
     * Basic constructor
     */
    public NotInBufferException () {
        super();
    }

    /**
     * Constructor with message
     * @param m    the message to include with the Excpetion
     */
    public NotInBufferException (String m) {
        super(m);
    }
}
