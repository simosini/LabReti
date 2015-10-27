import java.net.Socket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.io.IOException;

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

		try {
			int port = Integer.parseInt(args[0]);
			//ia = InetAddress.getByName("pc3-21");
			ia = InetAddress.getLocalHost();
			isa = new InetSocketAddress(ia, port);
			s.connect(isa);
			System.out.println("Porta locale: " + s.getLocalPort());
			System.out.println("Indirizzo: " + s.getInetAddress() + "; porta: " + s.getPort());
		
			InputStreamReader tastiera = new InputStreamReader(System.in);
			BufferedReader br = new BufferedReader(tastiera);
			String msg = br.readLine();
			OutputStream toServer = s.getOutputStream();
			toServer.write(msg.getBytes(), 0, msg.length());

			Thread.sleep(120*1000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
