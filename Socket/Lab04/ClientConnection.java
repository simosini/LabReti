import java.net.Socket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.io.IOException;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.io.OutputStream;

public class ClientConnection
{
	public static void main(String[] args)
	{
		Socket s = new Socket();
		InetAddress ia;
		InetSocketAddress isa;
		int buffer_dimension = 3;

		try {
			int port = Integer.parseInt(args[1]);
			ia = InetAddress.getByName(args[0]);
			isa = new InetSocketAddress(ia, port);
			s.connect(isa);
			System.out.println("Porta locale: " + s.getLocalPort());
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getPort());
		
			InputStreamReader tastiera = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(tastiera);
			OutputStream toServer = s.getOutputStream();

			byte buffer[] = new byte[buffer_dimension];
			InputStream fromServer = s.getInputStream();	

			while (true) {
				String msg = br.readLine();
				if (msg.compareTo("") == 0) {
					System.out.println("Stringa nulla, chisura connessione.");
					toServer.write("EXIT".getBytes());

					int letti = fromServer.read(buffer);
					msg = new String(buffer, 0, letti);
					if (msg.compareTo("ACK") == 0) 
						System.out.println("Ricevuto ACK, chiusura client.");
					else
						System.out.println("Errore: aspettato ACK.");
					break;
				}
				toServer.write(msg.getBytes(), 0, msg.length());

				int letti = fromServer.read(buffer);
				msg = new String(buffer, 0, letti);
				if (msg.compareTo("ACK") == 0) 
					System.out.println("Ricevuto ACK, inviare un nuovo messaggio.");
				else
					System.out.println("Errore: aspettato ACK.");
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				s.close();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
}
