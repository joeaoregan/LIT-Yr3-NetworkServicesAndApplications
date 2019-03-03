/* 
   Joe O'Regan
   K00203642
   Lab 1 Part 1
   15/09/2016 
*/

/* APPLICATION SET-UP AND PARAMETER PASSING

Declare standard functions and constants. 
The header file "Practical.h" contains function prototypes */

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

/* Check number of arguments, and error message displayed if not correct. 
The servers IP address, the string to echo, and optional server port argument, 
if there's no port echo protocol port 7 is used. */

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
  
  
  /* SOCKET CREATION
  
  The socket() function creates a socket for IPv4 using stream-based TCP protocol. 
  The function returns an integer handle if successful. 
  If failure returns -1, and error handling function displays message.
  
  Create a reliable, stream socket using TCP
  socket() = function to create a socket
  AF_INET: Socket is for IPv4, SOCK_STREAM: Using stream-based protocol, 
  IPPROTO_TCP: TCP protocol */
  
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	// IPv4, stream based protocol called TCP
  if (sock < 0)											// -1 returned if socket is unsucessful
    DieWithSystemMessage("socket() failed");			// Error function outputs message


  /* PREPARING ADDRESSING DETAILS AND ESTABLISHING THE CONNECTION WITH THE SERVER
  
  A structure is used to hold the address and port. Any parts not set are padded with zeros. */

  // Construct the server address structure
  struct sockaddr_in servAddr;            // Server address - struct to store info
  memset(&servAddr, 0, sizeof(servAddr)); // Zero out structure - any parts not set
  
  /* Address family, internet address, and port number are set. 
  The server's internet address is converted to 32-bit binary. 
  And the port number binary value is correctly formatted. */
  
  servAddr.sin_family = AF_INET;          // IPv4 address family
  // intet_pton(): Convert address to 32-bit binary (address family, server IP)
  int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr); // sin_addr: IP address
  if (rtnVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (rtnVal < 0)
    DieWithSystemMessage("inet_pton() failed");
  servAddr.sin_port = htons(servPort);    // Server port added in correct format to structure
  //htons(): host to network short, checks binary value is correct format
  
  /* Connect() function connects this socket with the socket with the address and port in the 
  sockaddr_in structure. Pointer to sockaddr_in is cast sockaddr* type, and the address data 
  structure size is included. */
  
  // Establish the connection to the echo server
  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
  // sockaddr_in cast to sockaddr*, size of address structure is supplied
    DieWithSystemMessage("connect() failed");

  
  /* SENDING DATA
  
  The argument length string is saved. 
  Send() needs the address of the first byte and how many bytes to send of the string, 
  and returns the number of bytes sent, or -1 if unsuccessful with error messages 
  for different fails. */

  size_t echoStringLen = strlen(echoString); // Determine input length of string entered at command line

  // Send the string to the server, up to the length of echoStringLen
  ssize_t numBytes = send(sock, echoString, echoStringLen, 0);	// returns number of bytes copied into buffer
  if (numBytes < 0)							// String did not send, -1 returned in case of failure
    DieWithSystemMessage("send() failed");
  else if (numBytes != echoStringLen)		// Wrong number of bytes sent
    DieWithUserMessage("send()", "sent unexpected number of bytes");
    
    
  /*  RECEIVING DATA 
  
  Number of bytes received is counted. A while loop is used to receive bytes until we have the 
  same amount that was sent, it's not guaranteed to run only once. 
  Recv() returns the number of bytes copied, -1 if it fails, or 0 if the TCP connection is closed at the other end. 
  The terminating character added to the end of received data, and data from the server is output 
  followed by a newline. */

  // Receive the same string back from the server
  unsigned int totalBytesRcvd = 0; 		// Count of total bytes received
  fputs("Received: ", stdout);     		// Setup to print the echoed string
  // Need to repeatedly receive bytes, until we receive as many as we sent
  while (totalBytesRcvd < echoStringLen) {
    char buffer[BUFSIZE]; // I/O buffer
    /* Receive up to the buffer size (minus 1 to leave space for
     a null terminator) bytes from the sender */
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0)					// -1 returned for failure
      DieWithSystemMessage("recv() failed");
    else if (numBytes == 0)				// Return 0 if app at other end closed the TCP connection
      DieWithUserMessage("recv()", "connection closed prematurely");
    totalBytesRcvd += numBytes; 		// Keep tally of total bytes received
    buffer[numBytes] = '\0';    		// Terminate the string! Add terminating null character
    fputs(buffer, stdout);      		// Print the echo buffer to standard output
  }

  fputc('\n', stdout); 					// Print a final linefeed


  /* TERMINATING CONNECTION AND EXITING CLEANLY
  
  Close() informs server communication has finished. And the program terminates */
  
  close(sock);		// Inform the remote socket communication has ended
  					// and deallocate local resources
  exit(0);			// Terminate the application
}
