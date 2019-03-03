/* APPLICATION SET-UP AND PARAMETER PASSING */

#include <stdio.h>						// functions such as printf
#include <stdlib.h>						// functions for memory allocation
#include <string.h>						// string handling
#include <sys/types.h>					// contains types
#include <sys/socket.h>					// internet protocol family
#include <netinet/in.h>					// internet address family
#include <arpa/inet.h>					// definitions for internet operatives, inet_ntop()
#include "Practical.h"					// header file with function prototypes

static const int MAXPENDING = 5; 		// Maximum outstanding connection requests

int main(int argc, char *argv[]) {		// run on command line = "echoSvr <port>";argc = 2 command and parameter
										// -  argv[0] = echoSvr and argv[1] = <port>

  if (argc != 2) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Server Port>");
  // convert port number from string to numeric with atoi(), a 0 will cause an error when bind() is called 
  in_port_t servPort = atoi(argv[1]); 	// First arg:  local port ascii to integer


  /* SOCKET CREATION */

  // Create a stream socket for incoming connections
  int servSock; 		// Socket descriptor for server
  if ((servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    DieWithSystemMessage("socket() failed");

  /* Construct local address structure for server
     The server needs to be associatedd with an address and port number
     So that the client can connect to the right place 
	 sockaddr_in:- any address assigned to the server will do */
  struct sockaddr_in servAddr;                  // Local address; internet socket address structure
  memset(&servAddr, 0, sizeof(servAddr));       // Zero out structure
  servAddr.sin_family = AF_INET;                // IPv4 address family
  
  /* INADDR_ANY:- Wildcard address for desired internet address
     convert address and port to network byte order using htonl() & htons() */
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Any incoming interface; host to network long[integer]
  servAddr.sin_port = htons(servPort);          // Local port; host to network short[integer]

  /* Bind associates server with the local address and port (server specifies its own address)
     The server has to specify its own address to bind
     The client and server have to agree on this address to connect
     Sockets can fail if another socket is using the port or if privileges required to bind to a certain port */
  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0) 
  												// cast servaddr as generic socket address structure
    DieWithSystemMessage("bind() failed");		// other socket using port num / privileges required to use port

  /* Listen() tells TCP implementation to allow incoming connections from clients
     before the listen() call incoming connection requests are silently rejected */
  // Mark the socket so it will listen for incoming connections
  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");


  /* REPEATEDLY TAKE CARE OF INCOMING CONNECTIONS 
  
     Instead of sending & receiving on the socket, server calls accept(), which blocks
	 until an incoming connection is made to the servers port
	 accept() returns a new socket descriptor, which is already connected */

  for (;;) { // Run forever
    struct sockaddr_in clntAddr; // Store client address
    // Set length of client address structure (in-out parameter)
    socklen_t clntAddrLen = sizeof(clntAddr);	// clntAddrLen: length of clntAddr

    // Wait for a client to connect to listening socket's port number
    // Arg1 socket descriptor
    // Arg2 soccaddr_in structure
    // Arg3 pointer to length of the structure
    int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
    if (clntSock < 0)
      DieWithSystemMessage("accept() failed");

    // clntSock is connected to a client!
	// clntAddr: internet address and port number of the connecting client
    char clntName[INET_ADDRSTRLEN]; // String to contain client address
    // inet_ntop(): converts address from binary to dotted-quad string
    if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
        sizeof(clntName)) != NULL)
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
    else
      puts("Unable to get client address");

    HandleTCPClient(clntSock);	// Receives data on the given socket and sends it back on the same socket,
    							// iterating as long as recv() is receiving data.
  }
  // NOT REACHED
}
