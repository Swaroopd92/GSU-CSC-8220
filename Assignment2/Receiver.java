package hw2Receiver;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

public class Receiver {

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		try {

			String pkt;
			String ack = "1";
			ServerSocket welcomeSocket = new ServerSocket(456);

			while (true) {
				// Connect to Router
				Socket connectionSocket = welcomeSocket.accept();
				System.out.println("\n\nReceiver: Router connected");
				BufferedReader inFromRouter = new BufferedReader(
						new InputStreamReader(connectionSocket.getInputStream()));

				// Read from Router
				pkt = inFromRouter.readLine();

				DataOutputStream outToRouter = new DataOutputStream(connectionSocket.getOutputStream());
				if (pkt.contains("Packet")) {
					System.out.println("FROM Router: " + pkt + '\n');
					// Sending the acknowledgement to Router
					outToRouter.writeBytes(ack + '\n');
				} else {
					ack = "nack";
					outToRouter.writeBytes(ack + '\n');
				}
			}

		} catch (Exception e) {
			// TODO: handle exception
			System.out.println(e.getMessage());
		}

	}

}
