import org.zeromq.ZMQ;

public class ServerThread extends Thread {

    public void run() {
        System.out.println("ZeroMQ thread: Starting ZeroMQ server");
        ZMQ.Context context = ZMQ.context(1);
        ZMQ.Socket publisher = context.socket(ZMQ.PUB);
        publisher.bind("tcp://*:5445");
        while (true) {
            publisher.send("hello from java!!", 0);
            System.out.println("ZeroMQ thread: value published");
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println("ZeroMQ thread: sleep exception");
            }
        }
    }

    public static void main(String args[]) {
        (new ServerThread()).start();
        while(true) {
        	System.out.println("Main thread: idle");
        	try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                System.out.println("Main thread: sleep exception");
            }
        }
    }

}