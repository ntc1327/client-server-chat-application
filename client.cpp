#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

// constant used for the quit string, not applicable for char comparison
const string QUITS = "!QUIT";

// creates a socket and the necessary tools needed to connect
// most of this comes from the provided sockets tutorial
// sIP is the given IP Address, used to get the host IP
// port is the given port, needed to convert to network byte order
sockaddr_in createSocket(char *sIP, int port) {
	struct hostent *hostServer = gethostbyname(sIP);
        sockaddr_in sendSockAddr;
        bzero((char*) &sendSockAddr, sizeof(sendSockAddr));
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*) *hostServer->h_addr_list));
        sendSockAddr.sin_port = htons(port);
	cout << "Socket created" << endl;
	return sendSockAddr;
}

// simply takes the client socket descriptor and closes the connection
void closeConnection(int cSockDes) {
	close(cSockDes);
        cout << "Connection closed";
} 

int main(int argc, char *argv[]) {
	// needs IP Address and port number to effectively run program
	if (argc != 3) {
		cout << "Use \"executable ip_address port\" to execute properly" << endl;
		exit(0);
	}

	// gathers the IP address, port number, and create the message
	char *sIP = argv[1];
	int port = atoi(argv[2]);
	char message[1000];

	// gets the socket address and gets the client's socket descriptor
	sockaddr_in createdSocket = createSocket(sIP, port);
	int cSockDes = socket(AF_INET, SOCK_STREAM, 0);

	// connects the client to the server
	int status = connect(cSockDes, (sockaddr*) &createdSocket, sizeof(createdSocket));
	cout << "Connecting to server..." << endl;
	
	// gives error if there was one, otherwise outputs success messages
	if (status < 0) {
		cout << "Error connecting to socket" << endl;
		exit(0);
	}
	else {
		cout << "Connected to server" << endl;
		cout << "Server is waiting for your response..." << endl;
	}

	// continues until !QUIT command breaks it
	while (1) {
		string input;
		
		// prompts for input
		cout << ">";
		getline(cin, input);
	
		// sets the buffer, copies the input, and sends the message
		memset(&message, 0, sizeof(message));
		strcpy(message, input.c_str());
		send(cSockDes, (char*) &message, strlen(message), 0);

		// breaks if input was !QUIT command
		if (input == QUITS) {
			break;
		}

		// waits for server to respond, sets buffer again before doing so
		cout << "Waiting for server..." << endl;
		memset(&message, 0, sizeof(message));
		recv(cSockDes, (char*) &message, sizeof(message), 0);
		
		// also can receive !QUIT message from the server
		if (!strcmp(message, "!QUIT")) {
			cout << "Server has closed the session" << endl;
			break;
		}
		
		// outputs the server's message
		cout << "Server: " << message << endl;
	}	
	// closes the client's connection
	closeConnection(cSockDes);
	return 0;
}














