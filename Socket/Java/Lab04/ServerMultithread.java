import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class ServerMultithread
{
	public static void main(String[] args) 
	{
		ServerSocket s = null;
		Socket toClient = null;
		RisorsaCondivisa counter = new RisorsaCondivisa();
		int clientID = 0;

		try {
			s = new ServerSocket(0);
			System.out.println("InetAddress: " + InetAddress.getLocalHost());
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getLocalPort());
			while(true) {
				toClient = s.accept();
				Thread subServ = new ServerIterativo(toClient, ++clientID, counter);
				subServ.start();
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				//no client can communicate with server
				s.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
