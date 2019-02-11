/*
*  Joe O'Regan
*  K00203642
*  Network Services and Applications
*  Take Home Assignment
*  24/11/2016
*
*  Server Socket
*/

#include <stdio.h>      	// printf() & fprintf()
#include <sys/socket.h> 	// socket(), connect(), send(), & recv()
#include <arpa/inet.h>  	// sockaddr_in & inet_addr()
#include <stdlib.h>		// random numbers, atoi() & exit()
#include <string.h>     	// memset()
#include <unistd.h>		// Close()
#include <ctype.h>		// Check for whitespace characters
#include <sys/types.h>		// ssize_t
#include <netinet/in.h>		// sockaddr_in
#include <time.h>		// rand numbers seed
#include "Practical.h"		// Contains prototypes for error checking

#define TCP_PORT 48031		// Default Port Number

int main(int argc, char *argv[]) {								// run on command line = "echoSvr <port>";
												// argc = 2 command and parameter-  argv[0] = echoSvr and argv[1] = <port>
	static const int MAXPENDING = 5;							// Maximum outstanding connection requests
	char msgReceived[96];									// Request string received from client - split
	char greeting[96];									// Greeting to send to client
	char msgSplit[96];									// Use to parse request string
	char* reqStr1, *reqStr2, *reqStr3, *reqStr4, *reqStr5, *gibberish;			// split the request string in separate strings
	char buffer[BUFSIZE]; 									// Buffer
	char outcome[40];									// Send the outcome, OK / Error
	int servSock; 										// Socket descriptor for server, handles incoming connections
	ssize_t numBytesSent, numBytesRcvd;							// Number of bytes sent to client, and received from client
	time_t t;										// For random number generation
 
	if (argc < 1 || argc > 2) {
		DieWithUserMessage("Parameter(s)", "[<Server Port>]");				// Check Arguments
	}

	in_port_t servPort = (argc == 2) ? atoi(argv[1]) : TCP_PORT;				// Use default port if none entered

	if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {			// Create socket
		DieWithSystemMessage("socket() failed");
	}

	// Construct local address structure
	struct sockaddr_in servAddr;								// Local address; internet socket address structure
	memset(&servAddr, 0, sizeof(servAddr));							// Zero out structure
	servAddr.sin_family = AF_INET;								// IPv4 address family
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);						// Any incoming interface; host to network long[integer]
	servAddr.sin_port = htons(servPort);							// Local port; host to network short[integer]
	socklen_t addrSize = sizeof(servAddr);							// Size of structure to hold address

  	// Bind to the local address
	if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) {		// Cast servaddr as generic socket address structure
		DieWithSystemMessage("bind() failed");
	}
	
	/* GETSOCKNAME */

	getsockname(servSock, (struct sockaddr *) &servAddr, &addrSize);			// Get address and port of local socket
	printf("listening on %s:%d\n", inet_ntoa(servAddr.sin_addr), servPort);
  
	if (listen(servSock, MAXPENDING) < 0) {							// Socket listens for incoming connections
		DieWithSystemMessage("listen() failed");
	}
	
	/* LOOP */

	for (;;) {										// Infinite loop
		int proceed = 0;								// Decides if rest of sends / recvs are necessary
		struct sockaddr_in clntAddr;							// Client address structure
		socklen_t clntAddrLen = sizeof(clntAddr);					// Set length of client address structure (in-out parameter)
	
		// Wait for a client to connect
		int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
		if (clntSock < 0) {
			DieWithSystemMessage("accept() failed");
		}
	
		// clntSock is connected to a client!
		char clntName[INET_ADDRSTRLEN];							// String to contain client address
		if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL) {
			printf("NEW CONNECTION\n\n");
			printf("Connected to: %s-%d, \n", clntName, ntohs(clntAddr.sin_port));	// client address / port
		} else {
			puts("Unable to get client address");
		}
	
		/* RECEIVE - Request String & Parse It */

		numBytesRcvd = recv(clntSock, buffer, BUFSIZE - 1, 0);
		if (numBytesRcvd < 0) {
			DieWithSystemMessage("recv() failed");
		}
		
		buffer[numBytesRcvd] = '\0';							// Terminate string
		printf("Request String: %s\n", buffer);
		printf("Number of bytes received: %lu\n", numBytesRcvd);
		
		/* PARSE - Request String */
		
		sprintf(msgReceived, "%s", buffer);						// copy buffer to another string to split
		char* server = "netsvr";							// Stored server name to compare
		char* requestType = "type0";							// Stored request type to compare
		char* username = "jaoregan";							// Stored username to compare
		char port[20];
		sprintf(port, "%d", ntohs(servAddr.sin_port));					// Server Port from input or stored default
		
		sprintf(msgSplit, "%s", msgReceived);
		reqStr1 = strtok (msgReceived, " ,-");						// Server
		reqStr2 = strtok (NULL, " ,-");							// Request type
		reqStr3 = strtok (NULL, " ,-");							// Username
		reqStr4 = strtok (NULL, " ,-");							// IP Address
		reqStr5 = strtok (NULL, "\r\n,-");						// Port
		gibberish = strtok (NULL, " ,-");						// Take any gibberish at end of string
		
		// Check format and set greeting to send
		printf("PARSED REQUEST STRING:\n");
		if (strcmp(reqStr1, server) == 0 && strcmp(reqStr2, requestType) == 0 && strcmp(reqStr3, username) == 0 
		&& strcmp(reqStr4, clntName) == 0 && strcmp(reqStr5, port) == 0) {
			printf("Server:\t\t%s\n", server);
			printf("Request Type:\t%s\n", requestType);
			printf("Username:\t%s\n", username);
			printf("IP Address:\t%s\n", clntName);
			printf("Port:\t\t%s\n\n", port);
			
			sprintf(greeting, "Hello %s-%d, welcome to the netsvr server\r\n", 
				clntName, ntohs(clntAddr.sin_port));				// client address / port
		} else {
			printf("Request string incorrectly formatted\n");
			
			sprintf(greeting, "Error not formatted right!\r\n");			// Request String not correct
			
			// Check input against store values
			printf("reqStr1: %s Vs %s\n", reqStr1, server);
			printf("reqStr2: %s Vs %s\n", reqStr2, requestType);
			printf("reqStr3: %s Vs %s\n", reqStr3, username);
			printf("reqStr4: %s Vs %s\n", reqStr4, clntName);
			printf("reqStr5: %s Vs %s\n", reqStr5, port);
			
			proceed = 1;								// No need for communciation after greeting
		}

		printf("Greeting: %s\n", greeting);						// Display the greeting
		
		/* SOCKET COMMUNICATION */
		
		// SEND - Greeting
		numBytesSent = send(clntSock, greeting, strlen(greeting), 0);			// Send the greeting
		if (isspace(greeting[0])) {							// Check for space at beginning of a line
			DieWithSystemMessage("Syntax error");
		}
		//if (strncmp(greeting, " ", 1) == 0)						// Old version, check for space
		//  DieWithSystemMessage("Syntax error");
		else if (numBytesSent < 0) {
			DieWithSystemMessage("send() failed");
		} else if (numBytesSent != strlen(greeting)) {
			DieWithUserMessage("send()", "sent unexpected number of bytes");
		}
	
		// CONDITIONAL
		if (proceed == 0) {								// Continue if the greeting is positive
			printf("Bytes sent: %lu\n", numBytesSent);				// Number of bytes in the greeting
	
			// RECEIVE - Num of bytes
			numBytesRcvd = recv(clntSock, buffer, strlen(buffer), 0);
			if (numBytesRcvd < 0) {
				DieWithSystemMessage("recv() failed");
			}

			// SEND - Random number of bytes
			srand((unsigned) time(&t));						// initialise random number
			int randomBytes = (rand() % 50) + 1;					// random number between 1 and 50
			printf("Random Bytes to send: %d\n", randomBytes);
			char message2[randomBytes];
			strncpy(message2, greeting,randomBytes);				// Send back random bytes of request string
			strcat(message2,"\r\n");						// Ad end-of-line terminator

			numBytesSent = send(clntSock, message2, strlen(message2), 0);
			if (isspace(message2[0])) {						// Check 1st character for whitespace
				DieWithSystemMessage("Syntax error");
			}

			// RECEIVE - Num of bytes
			numBytesRcvd = recv(clntSock, buffer, BUFSIZE - 1, 0);
			if (numBytesRcvd < 0) {
				DieWithSystemMessage("recv() failed");
			}
			buffer[numBytesRcvd] = '\0';						// Terminate the string
			printf("Size of data received by client: %s \n", buffer);
			int clientReturnValue = atoi(buffer);					// Convert bytes client received to integer
			printf("Client bytes returned: %d\n", clientReturnValue);		// Num bytes received by client
			
			/* COOKIE */

			// SEND - Outcome / Cookie
			srand((unsigned) time(&t));						// initialise random number
			int cookie = rand() % 1000000000;					// random number between 0 and 999999999

			/* Decide Outcome */

			if (clientReturnValue == randomBytes) {					// If the bytes are equal
				sprintf(outcome, "OK %d", cookie);				// amounts match
			} else {								// amounts don't match
				sprintf(outcome, "Returned value different %d", cookie);
			}

			numBytesSent = send(clntSock, outcome, strlen(outcome), 0);		// Send outcome to client
			if (isspace(outcome[0])) {						// Check 1st character for whitespace
				DieWithSystemMessage("Syntax error");
			} else if (numBytesSent < 0) {
				DieWithSystemMessage("send() failed");
			}
			printf("Outcome and Cookie: %s\n\n", outcome);
		}
		
		close(clntSock);								// Close client socket
	}											// end of infinite for loop
}
