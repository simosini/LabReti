import java.net.InetAddress;
import java.net.Socket;

import java.io.InputStream;
import java.io.OutputStream;

public class ServerIterativo extends Thread
{
	private Socket toClient;
	private int clientID;
	private RisorsaCondivisa counter;

	public ServerIterativo(Socket toClient, int clientID, RisorsaCondivisa counter)
	{
		this.toClient = toClient;
		this.clientID = clientID;
		this.counter = counter;
	}

	public void run() 
	{
		int buffer_dimension = 100;
		int count = 0;

		try {
			System.out.println("Indirizzo client: " + toClient.getInetAddress() + "; porta: " + toClient.getPort() + " si e' connesso.");

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
				System.out.println("Ricevuta stringa \"" + msg + "\" di " + letti + " byte dal client " + clientID + ".");
				count++;
				System.out.println("Invio ACK.");
				osToClient.write("ACK".getBytes());
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				//specific client can't communicate with server
				System.out.println("Client " + clientID + " ha chiuso la connessione dopo " + count + " messaggi");
				System.out.println("Totale messaggi ricevuti: " + counter.incrementa(count));
				toClient.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
