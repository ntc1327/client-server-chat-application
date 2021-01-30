#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;

// constant used for the quit string, not applicable for char comparison
const string QUIT = "!QUIT";

// creates a socket and the necessary tools needed to connect
// most of this comes from the provided sockets tutorial
// sIP is the given IP address, needed to set up the socket on the proper IP address
// port is the given port, needed to convert to network byte order
sockaddr_in createSocket(char *sIP, int port) {
	sockaddr_in sAddr;
        bzero((char*) &sAddr, sizeof(sAddr));
        sAddr.sin_family = AF_INET;
        sAddr.sin_addr.s_addr = inet_addr(sIP);
        sAddr.sin_port = htons(port);
	return sAddr;
}

// simply outputs the client's message and prompts for input
void messageAndPrompt(char message[1000]) {
	cout << "Client: " << message << endl;
	cout << ">";
}

// closes the client connection and outputs message saying so
void clientCloseConnection(int newSockDes) {
	close(newSockDes);
        cout << "End Client connection" << endl;
}

int main(int argc, char *argv[]) {
	// program needs IP address and port number to execute properly
	if (argc != 3) {
		cerr << "Use \"executable ip_address port\" to execute" << endl;
		exit(0);
	}
	
	// gathers the input for the IP address and port number and creates the message variable
	int port = atoi(argv[2]);
	char *sIP = argv[1];
	char message[1000];

	// gets the socket address
	sockaddr_in sAddr = createSocket(sIP, port);

	// creates the socket descriptor, outputs error if there was one
	int sSockDes = socket(AF_INET, SOCK_STREAM, 0);
	if (sSockDes < 0) {
		cout << "Error establishing the server socket" << endl;
		exit(0);
	}
	
	// determines if server socket can bind, outputs error if not
	int canBind = bind(sSockDes, (struct sockaddr*) & sAddr, sizeof(sAddr));
	if (canBind < 0) {
		cout << "Error binding socket to local address" << endl;
                exit(0);
	}
	
	// once program hits this loop, needs Ctrl-c to stop waiting for connection
	while (1) {
		// listens for client
		cout << "Waiting for a client to connect..." << endl;
		listen(sSockDes, 5);
		
		// sets up new socket address to accept incoming connection
		sockaddr_in newSAddr;
		socklen_t newSAddrSize = sizeof(newSAddr);

		// accepts an incoming connection, outputs an error if failed to accept
		int newSockDes = accept(sSockDes, (sockaddr *)&newSAddr, &newSAddrSize);
		if (newSockDes < 0) {
			cout << "Error accepting request from client" << endl;
			exit(1);
		}
		
		// outputs success message due to no error
		cout << "Connected with client" << endl;

		// continues until !QUIT command breaks it
		while (1) {
			string input;

			// waits to receive first message from client, sets buffer
			cout << "Waiting for client..." << endl;
			memset(&message, 0, sizeof(message));
			recv(newSockDes, (char*) &message, sizeof(message), 0);
		
			// breaks if client used !QUIT command
			if (!strcmp(message, "!QUIT")) {
				cout << "Client has quit the session" << endl;
				break;
			}
		
			// outputs client message and prompts for input
			messageAndPrompt(message);
			getline(cin, input);

			// sets buffer, copies the input, then sends message based on if !QUIT was input
			memset(&message, 0, sizeof(message));
			strcpy(message, input.c_str());
			if (input == QUIT) {
				send(newSockDes, (char*) &message, strlen(message), 0);
				break;
			}
			else {
				send(newSockDes, (char*) &message, strlen(message), 0);
			}
		}
		// runs if client closes connection
		clientCloseConnection(newSockDes);	
	}
	// closes server connection if ever needs to get here
	close(sSockDes);
	return 0;
}
