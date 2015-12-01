import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ServerMultithread
{
	public static void main(String[] args) 
	{
		ServerSocket s = null;
		Socket toClient = null;

		try {
			s = new ServerSocket(0);
			System.out.println("InetAddress: " + InetAddress.getLocalHost());
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getLocalPort());
			while(true) {
				toClient = s.accept();
				Thread subServ = new ServerIterativo(toClient);
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
