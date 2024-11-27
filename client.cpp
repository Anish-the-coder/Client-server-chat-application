#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<thread>
#include<string>

//it basically similar to a whatsapp grp

using namespace std;

#pragma comment(lib, "ws2_32.lib")

/*step
		initailize winsock

		create socket

		connect to the server

		send/recv

		close the socket
*/

bool Initailize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

//sent multiple msg
void SendMsg(SOCKET s) {
	cout << "Enter your chat name: " << endl;  //name of the sender
	string name;
	getline(cin, name);
	string message;

	while (1) {
		getline(cin, message);
		string msg = name + " : " + message;
		
		//contineous sending of msg
		int bytesent = send(s, msg.c_str(), msg.length(), 0);
		
		if (bytesent == SOCKET_ERROR) {
			cout << "error in sending message" << endl;
			break;
		}

		//message quit to disconnect from the server.
		if (message == "quit") {
			cout << "disconnecting from the server" << endl;
			break;
		}		
	}
    closesocket(s);
	WSACleanup();
}

//recieve multiple msg
void RecieveMsg(SOCKET s) {
	char buffer[4096];
	int recvlength;
	string msg = "";
	while (1) {
		recvlength = recv(s, buffer, sizeof(buffer), 0);
		if (recvlength <= 0) {
			cout << "disconnected from the server" << endl;
			break;
		}
		else {
			msg = string(buffer, recvlength);
			cout << msg << endl;
		}
	}

	closesocket(s);
	WSACleanup();
}

int main() {
	cout << "client program started" << endl;

	if (!Initailize()) {
		cout << "initailize winsock failed" << endl;

		return 1;
	}

	SOCKET s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cout << "invalid socket created" << endl;
	}

	int port = 12335;
	string serveraddress = "127.0.0.1";
	//connecting on local host since both client and server are running on the same host machine
	
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	inet_pton(AF_INET, serveraddress.c_str(), &(serveraddr.sin_addr));

	if (connect(s, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "not able to connect to server" << endl;
		cout << ": " << WSAGetLastError();
		closesocket(s);
		WSACleanup();
		return 1;
	}

	cout << "successfully connected to server" << endl;

	//to continuously send and recieve msg 
	thread senderthread(SendMsg, s);  
	thread receiver(RecieveMsg, s);

	senderthread.join();
	receiver.join();

	return 0;

}