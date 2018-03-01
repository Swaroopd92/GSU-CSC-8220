package hw2Router;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.TimeUnit;

public class Router {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		try {

			String pkt;
			String ack;

			ServerSocket welcomeSocket = new ServerSocket(123);

			while (true) {
				// Connect to Sender
				Object obj = new Object();
				long t = (long) (Math.random() * 10000) % 10;
				int p = (int) (Math.random() * 10000) % 100;
				Socket connectionSocket = welcomeSocket.accept();
				BufferedReader inFromSender = new BufferedReader(
						new InputStreamReader(connectionSocket.getInputStream()));

				System.out.println("\n\nRouter: Sender connected\n\n");

				// Read data from sender
				pkt = inFromSender.readLine();
				System.out.println("Router: " + pkt);

				// Connect to Receiver
				InetAddress hostName;
				hostName = InetAddress.getLocalHost();
				System.out.println("Receiver's address: " + hostName);

				// Send packet to Receiver
				// obj.wait(t);
				TimeUnit.MILLISECONDS.sleep(t);
				System.out.println("Delay: " + t + "ms");
				System.out.println("Drop probability: " + p + "%");
				Socket routerSocket = new Socket(hostName, 456);
				DataOutputStream outToReceiver = new DataOutputStream(routerSocket.getOutputStream());
				if (p > 19) {
					outToReceiver.writeBytes(pkt + '\n');
					System.out.println("FROM Sender: " + pkt + '\n');
				} else {
					pkt = "nack";
					outToReceiver.writeBytes(pkt + '\n');
					System.out.println("Packet dropped ");
				}

				// Receiving packet from Receiver
				BufferedReader inFromReceiver = new BufferedReader(
						new InputStreamReader(routerSocket.getInputStream()));
				ack = inFromReceiver.readLine();
				routerSocket.close();

				// Sending the acknowledgement to sender
				DataOutputStream outToSender = new DataOutputStream(connectionSocket.getOutputStream());
				outToSender.writeBytes(ack + '\n');
				System.out.println("FROM Receiver: " + ack + '\n');
			}
		} catch (Exception e) {
			// TODO: handle exception
			System.out.println(e.getMessage());
		}

	}

}
