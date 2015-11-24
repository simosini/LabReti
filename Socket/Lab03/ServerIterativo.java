import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class ServerIterativo
{
	public static void main(String[] args) 
	{
		ServerSocket s = null;
		Socket toClient = null;
		int buffer_dimension = 100;

		try {
			s = new ServerSocket(0);
			System.out.println("InetAddress: " + InetAddress.getLocalHost());
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getLocalPort());
			toClient = s.accept();
			System.out.println("Indirizzo client: " + toClient.getInetAddress() + "; porta: " + toClient.getPort());

			byte buffer[] = new byte[buffer_dimension];
			InputStream is = toClient.getInputStream();		
			OutputStream osToClient = toClient.getOutputStream();

			while (true) {
				int letti = is.read(buffer);
				String msg = new String(buffer, 0, letti);
				if (msg.compareTo("EXIT") == 0) {
					System.out.println("Ricevuta richiesta di uscita, chiusura connessione.");
					System.out.println("Invio ACK.");
					osToClient.write("ACK".getBytes());
					break;
				}
				System.out.println("Ricevuta stringa \"" + msg + "\" di " + letti + " byte.");
				System.out.println("Invio ACK.");
				osToClient.write("ACK".getBytes());
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				//specific client can't communicate with server
				toClient.close();
				//no client can communicate with server
				s.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
