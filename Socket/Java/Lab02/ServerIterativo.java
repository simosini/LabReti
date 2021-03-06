import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

import java.io.InputStream;

public class ServerIterativo
{
	public static void main(String[] args) 
	{
		ServerSocket s = null;
		Socket toClient = null;
		int buffer_dimension = 100;

		try {
			s = new ServerSocket(0);
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getLocalPort());
			toClient = s.accept();
			System.out.println("Indirizzo client: " + toClient.getInetAddress() + "; porta: " + toClient.getPort());

			byte buffer[] = new byte[buffer_dimension];
			InputStream is = toClient.getInputStream();
			int letti = is.read(buffer);
			String msg = new String(buffer, 0, letti);
			System.out.println("Ricevuta stringa: " + msg + " di " + letti + " byte.");
			Thread.sleep(240*1000);
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				//specific client can't communicate with server
				toClient.close(); // chiude socket attiva ma puo accettare altre richieste connessioni
				//no client can communicate with server
				s.close(); // chiude server non accetta piu nessuna connessione
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
