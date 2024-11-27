#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<tchar.h>
#include<thread>
#include<vector>

using namespace std;

#pragma comment(lib, "ws2_32.lib")

/*
	initialize winsock library

	create the socket

	get ip and port

	bind the ip/port with the socket

	listen on the socket

	accept 

	receive and send

	close the socket

	cleanup the winsock
*/

bool Initialize() {
	WSADATA data;
	return WSAStartup(MAKEWORD(2, 2), &data) == 0;
}

void InteractWithClient(SOCKET clientSocket, vector<SOCKET> &clients) {
	//send/recv client

	cout << "client connected" << endl;
	char buffer[4096];	

	while (1) {
		int bytesrecvd = recv(clientSocket, buffer, sizeof(buffer), 0); //the msg that is sent is stored in the buffer

		if (bytesrecvd <= 0) {
			cout << "client disconnected" << endl;
			break;
		}
		string message(buffer, bytesrecvd);
		cout << "message from client: " << message << endl;
		
		//sending the msg to all the clients connected to the server
		for (auto client : clients) {			
			
			//except the sender itself
			if (client != clientSocket) {
				send(client, message.c_str(), message.length(), 0);
			}			
		}
	}

	auto it = find(clients.begin(), clients.end(), clientSocket);
	if (it != clients.end()) {
		clients.erase(it);
	}

	closesocket(clientSocket);
}

int main() {
	if (!Initialize()) {
		cout << "winsock initailization failed" << endl;
	}

	cout << "server program" << endl;

	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	/*parameter: AF_INET: ipv4, 
				 sock_stream(create a socket of type) : TCP
				 protocal: 0(let the service provider decide the protocal)

	*/

	//check if the socket is sucessfully created
	if (listenSocket == INVALID_SOCKET) {
		cout << "socket creation failed" << endl;
		return 1;
	}

	//create address structure
	int port = 12335;
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);

	//convert to ipaddress (0.0.0.0) put it inside the sin_family in binary form
	if (InetPton(AF_INET, _T("0.0.0.0"), &serveraddr.sin_addr) != 1) {
		cout << "setting address structure failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//bind the ip and port with the socket
	if (bind(listenSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr)) == SOCKET_ERROR) {
		cout << "bind failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//listen on the socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {  //it tells how many req added to the queue untill one is served.
		cout << "Listen failed" << endl;
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	cout << "server has started listening on port: " << port << endl;
	vector<SOCKET> clients;

	while(1) {  
		
		//so that we can accept contineous connection		
		//the accept will be put to halt until a new connection is recieved
		//accept the connection from the client
		
		SOCKET clientSocket = accept(listenSocket, nullptr, nullptr);   //specific to the client that it has connected now
		if (clientSocket == INVALID_SOCKET) {
			cout << "invalid client socket" << endl;
		}


		//main thread will be busy accepting connections
		//in a continuous manner using the while loop.
		//keep a record of all the connected clients.
		clients.push_back(clientSocket);
		thread t1(InteractWithClient, clientSocket, std::ref(clients)); 
		t1.detach();
		//seperating the connection from the main thread to seperate a thread
		//so that other connections btw then clients and the server can be maintained. 
	}	
	
	closesocket(listenSocket);

	return 0;
}