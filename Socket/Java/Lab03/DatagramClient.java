import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;

import java.io.BufferedReader;
import java.io.InputStreamReader;

class DatagramClient {
	public static void main(String[] args) {
		DatagramSocket socket = null;
		int buffer_dimension = 100;

		try {
			socket = new DatagramSocket();

			System.out.println("Indirizzo client: " + socket.getLocalAddress() + ":" + socket.getLocalPort() + ".");
			int port = Integer.parseInt(args[1]);
			InetAddress ia = InetAddress.getByName(args[0]);
			InputStreamReader tastiera = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(tastiera);

			while (true) {
				String msg = br.readLine();
				if (msg.compareTo("") == 0) {
					System.out.println("Stringa nulla: chiusura client.");
					break;
				}
				DatagramPacket p = new DatagramPacket(msg.getBytes(), msg.getBytes().length, ia, port);
				socket.send(p);

				byte[] buffer = new byte[buffer_dimension];
				DatagramPacket p_ack = new DatagramPacket(buffer, buffer.length);

				socket.receive(p_ack);				
				
				msg = new String(buffer, 0, p_ack.getLength());
				if (msg.compareTo("ACK") == 0) 
					System.out.println("Ricevuto ACK.");
				else
					System.out.println("Errore: aspettato ACK, ricevuto " + msg + ".");			
			}
		}
		catch (Exception ex) {
			ex.printStackTrace();
		}
		finally {
			socket.close();
		}
	}
}
