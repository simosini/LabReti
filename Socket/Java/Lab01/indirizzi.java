import java.net.InetAddress;
import java.net.UnknownHostException;

public class indirizzi
{
	public static void main(String[] args)
	{
		String nome_singolo = "www.unimi.it";
		String nome_multiplo = "www.google.it";

		try {
			System.out.println("Localhost associated with address " + InetAddress.getLocalHost());
			System.out.println();		
			
			InetAddress ia = InetAddress.getByName(nome_singolo);
			byte[] ndp = ia.getAddress();
			System.out.println("Name " + nome_singolo + " associated with address " + (ndp[0] & 0xff) + "." + (ndp[1] & 0xff) + "." + (ndp[2] & 0xff) + "." + (ndp[3] & 0xff));

			InetAddress[] iaa = InetAddress.getAllByName(nome_multiplo);
			System.out.println();
			System.out.println("All addresses for " + nome_multiplo + ":");
			for (int i = 0; i < iaa.length; i++)
				System.out.println("Name: " + iaa[i].getHostName() + " associated with address " + iaa[i].getHostAddress());
		}
		catch (UnknownHostException uhe) {
			uhe.printStackTrace();
		}
	}
}
