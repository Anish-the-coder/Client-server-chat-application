# Client-server-chat-application
### README for Client-Server Chat Application

---

## **Overview**
This is a simple client-server chat application built using **C++** and **WinSock2** library. The program allows multiple clients to connect to a server and exchange messages in a group-chat environment, similar to a WhatsApp group. The server broadcasts messages received from one client to all other connected clients. 

---

## **Features**
- **Multi-client support:** Multiple clients can connect to the server simultaneously.
- **Group messaging:** Messages sent by one client are broadcasted to all other clients.
- **Real-time communication:** Continuous message sending and receiving using threads.
- **Graceful disconnection:** Clients can disconnect by sending the `"quit"` message.

---

## **Setup Instructions**

### **Prerequisites**
1. Windows OS (WinSock2 library is used, which is specific to Windows).
2. C++ compiler supporting WinSock2 (e.g., Visual Studio).
3. Basic understanding of socket programming.

### **Steps to Run**
1. Clone the repository:
   ```bash
   git clone https://github.com/Anish-the-coder/Client-server-chat-application.git
   ```
2. Open the project in your preferred IDE (e.g., Visual Studio).
3. Build the **server** and **client** programs separately.
4. Run the **server** executable first to start listening for client connections.
5. Run the **client** executable(s) to connect to the server.

---

## **How It Works**

### **Server Workflow**
1. Initializes the **WinSock** library.
2. Creates a listening socket bound to an IP address and port.
3. Waits for incoming client connections.
4. Creates a new thread for each client to handle:
   - Receiving messages from the client.
   - Broadcasting messages to other connected clients.
5. Manages a list of all connected clients.
6. Cleans up disconnected clients and resources.

### **Client Workflow**
1. Initializes the **WinSock** library.
2. Connects to the server using the server's IP address and port.
3. Starts two threads:
   - **Sender Thread:** Continuously sends messages to the server.
   - **Receiver Thread:** Continuously receives messages from the server and displays them.
4. Gracefully disconnects by sending the `"quit"` message.

---

## **Usage**
1. Start the server program. It will begin listening on a specified port (default: `12335`).
2. Start one or more client programs and connect them to the server.
3. Enter a username for each client.
4. Begin chatting! Type messages and press Enter to send.
5. To disconnect, type `"quit"`.

---

## **Known Issues**
- Ensure the server is running before starting the clients.
- If a client sends an empty message, it might cause unintended behavior.
- The `recv` buffer is fixed at 4096 bytes; messages larger than this will be truncated.

---

Feel free to contribute to the project and make it better! 😊