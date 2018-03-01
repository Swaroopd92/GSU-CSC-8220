package hw2Sender;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.Socket;
import java.util.Scanner;

public class Sender {

	public static void main(String[] args) {
		// TODO Auto-generated method stub
			try {

				Scanner in = new Scanner(System.in);
				InetAddress hostName;
				hostName = InetAddress.getLocalHost();
				System.out.println("Router's address: " + hostName);

				String pkt = "Packet";
				int i, y;
				String ack = null;

				// BufferedReader inFromUser = new BufferedReader(new
				// InputStreamReader(System.in));

				System.out.println("\nEnter the number of packets: ");
				y = in.nextInt();
				in.close();

				// sentence = inFromUser.readLine();
				for (i = 1; i <= y; i++) {
					Socket senderSocket = new Socket(hostName, 123);
					DataOutputStream outToRouter = new DataOutputStream(senderSocket.getOutputStream());
					BufferedReader inFromRouter = new BufferedReader(new InputStreamReader(senderSocket.getInputStream()));

					outToRouter.writeBytes(pkt + '\n');
					System.out.println("FROM Sender: " + pkt + i + '\n');

					
					ack = inFromRouter.readLine();
					if(ack.contains("nack")) {
						i-=1;
					}

					senderSocket.close();
				}

			} catch (Exception e) {
				// TODO: handle exception
				System.out.println(e.getMessage());
			}

	}

}
