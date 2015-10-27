import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

public class ServerIterativo
{
	public static void main(String[] args) 
	{
		ServerSocket s;
		Socket toClient;
		try {
			s = new ServerSocket(0);
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getLocalPort());
			toClient = s.accept();
			System.out.println("Indirizzo client: " + toClient.getInetAddress() + "; porta: " + toClient.getPort());
			Thread.sleep(240*1000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
