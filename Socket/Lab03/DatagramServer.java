import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;

class DatagramServer {
		public static void main(String[] args) {
			DatagramSocket socket = null;
			int buffer_dimension = 100;

			try {
				socket = new DatagramSocket();

				System.out.println("Indirizzo server: " + socket.getLocalAddress() + ":" + socket.getLocalPort() + ".");

				while (true) {
					byte[] buffer = new byte[buffer_dimension];
					DatagramPacket p = new DatagramPacket(buffer, buffer.length);
					socket.receive(p);		
					String msg = new String(buffer, 0, p.getLength());	
					System.out.println("Ricevuto \"" + msg + "\" da " + p.getAddress() + ":" + p.getPort() + ".");

					DatagramPacket p_ack = new DatagramPacket("ACK".getBytes(), "ACK".getBytes().length, p.getAddress(), p.getPort());
					socket.send(p_ack);
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
