import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

public class virtual{

    public static void main(String[] args) {
        
        if (args.length != 3) {
            System.err.println("Usage: java virtual <ON_IP> <ON_PORT> <VN_UDP_PORT>");
            System.exit(1);
        }


        DatagramSocket socket = null;

        try {
           
            InetAddress oracleIP = InetAddress.getByName(args[0]);

            int oraclePort = Integer.parseInt(args[1]);
            int vn_port = Integer.parseInt(args[2]);

            System.out.println("Oracle Node IP: " + oracleIP.getHostAddress());
            System.out.println("Oracle Node Port: " + oraclePort);
            System.out.println("Virtual Node UDP Port: " + vn_port);
            socket = new DatagramSocket(vn_port);
        } 
        
        catch (IllegalArgumentException e) {
            System.err.println("Error: " + e.getMessage());
            System.exit(1);

        }finally {
            
        }
    }
}
