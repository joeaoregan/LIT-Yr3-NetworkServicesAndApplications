/* Application set-up and parameter passing */

#include <stdio.h>				// functions such as printf
#include <stdlib.h>				// functions for memory allocation, atoi() etc
#include <string.h>				// string handling, memset() etc
#include <unistd.h>				// access to POSIX o/s API, close()
#include <sys/types.h>			// contains types
#include <sys/socket.h>			// internet protocol family socket(), connect() etc
#include <netinet/in.h>			// internet address family, in_port_t (port number), sockaddr_in, sin_family
#include <arpa/inet.h>			// definitions for internet operatives, inet_pton()
#include "Practical.h"			// header file with function prototypes

int main(int argc, char *argv[]) {		// Pass arguments from the command line when running
// Output this message if the correct number of arguments are not entered
  if (argc < 3 || argc > 4) 			// Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",	// error message for incorrect arguments
        "<Server Address> <Echo Word> [<Server Port>]");
// Enter the server IP address, the string to send and the port number for the server
  char *servIP = argv[1];     			// First arg: server IP address (dotted quad)
  char *echoString = argv[2]; 			// Second arg: string to echo

  // Third arg (optional): server port (numeric).  7 is well-known echo port
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;	// if no port provide echo port 7 is used
  														// in_port_t (Port number)
  
  
  /* Socket Creation */
  
  // Create a reliable, stream socket using TCP
  // socket() = function to create a socket
  // AF_INET: Socket is for IPv4, SOCK_STREAM: Using stream-based protocol, IPPROTO_TCP: TCP protocol
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// IPv4, stream based protocol called TCP
  if (sock < 0)						// -1 returned if socket is unsucessful
    DieWithSystemMessage("socket() failed");		// Error function outputs message


  /* Preparing Addressing Details And Establishing The Connection With The Server */

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address - struct to store info
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure - any parts not set
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // intet_pton(): Convert address to 32-bit binary (address family, server IP)
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr); // sin_addr: IP address
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port added in correct format to structure
  //htons(): host to network short, checks binary value is correct format

  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
  // sockaddr_in cast to sockaddr*, size of address structure is supplied
    DieWithSystemMessage("connect() failed");

  
  /* Sending Data */

  size_t echoStringLen = strlen(echoString); // Determine input length of string entered at command line

  // Send the string to the server, up to the length of echoStringLen
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);	// returns number of bytes copied into buffer
  if (numBytes < 0)						// String did not send, -1 returned in case of failure
    DieWithSystemMessage("send() failed");
  else if (numBytes != echoStringLen)				// Wrong number of bytes sent
    DieWithUserMessage("send()", "sent unexpected number of bytes");
  

  // Display Server address message
  char serverName[INET_ADDRSTRLEN]; 				// String to contain server address
  if (inet_ntop(AF_INET, &servAddr.sin_addr.s_addr, serverName,
        sizeof(serverName)) != NULL)
  printf("Connecting To Server %s/%d\n", serverName, ntohs(servAddr.sin_port));
  printf("Sending Echo Word: \"%s", echoString);   
    

  /*  Receiving Data */

  // Receive the same string back from the server
  unsigned int totalBytesRcvd = 0; 		// Count of total bytes received
  fputs("\"\nReceived: ", stdout);     	// Setup to print the echoed string
  // Need to repeatedly receive bytes, until we receive as many as we sent
  while (totalBytesRcvd < echoStringLen) {
    char buffer[BUFSIZE]; // I/O buffer
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)				// -1 returned for failure
      DieWithSystemMessage("recv() failed");
    else if (numBytes == 0)			// Return 0 if app at other end closed the TCP connection
      DieWithUserMessage("recv()", "connection closed prematurely");
    totalBytesRcvd += numBytes; 	// Keep tally of total bytes received
    buffer[numBytes] = '\0';    	// Terminate the string! Add terminating null character
    fputs(buffer, stdout);      	// Print the echo buffer to standard output
  }

  fputc('\n', stdout); 				// Print a final linefeed


  /* Terminating Connection And Exiting Cleanly */
  
  close(sock);						// Inform the remote socket communication has ended
  									// and deallocate local resources
  exit(0);							// Terminate the application
}
