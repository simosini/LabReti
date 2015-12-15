import java.net.InetAddress;
import java.net.UnknownHostException;
import java.net.Socket;
import java.net.InetSocketAddress;

public class explicit_binding
{
	public static void main(String[] args)
	{
		Socket sClient = new Socket();
		try {
			InetAddress ia = InetAddress.getLocalHost();
			InetSocketAddress isa = new InetSocketAddress(ia, 0);
			sClient.bind(isa);
			System.out.println("OS allocated port number " + sClient.getLocalPort());
			Thread.sleep(120*1000);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
