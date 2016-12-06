/*************************************** 
*  Joe O'Regan
*  K00203642
*  Network Services and Applications
*  Take Home Assignment
*	 24/11/2016
*
*  Client Socket
***************************************/

/* Setup */

#include <stdio.h>      								// printf() & fprintf()
#include <sys/socket.h> 								// socket(), connect(), send(), & recv()
#include <arpa/inet.h>  								// sockaddr_in & inet_addr()
#include <stdlib.h>     								// atoi() & exit()
#include <string.h>     								// memset()
#include <unistd.h>     								// close()
#include <ctype.h>											// Check for whitespace characters
#include "Practical.h"									// Contains prototypes for error checking

#define BUFFSIZE 96   									// Size of receive buffer
#define TCP_PORT 48031									// Default port number

int main(int argc, char *argv[])
{
  int sock;                             // Socket descriptor
  struct sockaddr_in servAddr, cliAddr; // Echo server address
  char *servIP;                 				// Server IP address (dotted quad)
  char sendStr[96];                 		// IP and port
  char buffer[BUFFSIZE];     	  	  		// Buffer for echo string
  unsigned int sendStrLen;      	  		// Length of string
  int bytesRcvd, totalBytesRcvd;   	  	// Bytes read in single recv() & total bytes read
  socklen_t addrSize = sizeof(cliAddr);

  if ((argc < 3) || (argc > 4)) 	  	  // Check correct number of arguments
    DieWithUserMessage("Parameter(2)", "<Server IP> <Request String> [<Server Port>]");

  servIP 						 = argv[1];         // First arg: server IP address (dotted quad)
	char* reqStr 			 = argv[2];					// request string = the 2nd parameter entered
	in_port_t servPort = (argc == 4) ? atoi(argv[3]) : TCP_PORT; // Port entered or default
	
  if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)	 // Create stream socket, TCP
    DieWithSystemMessage("socket() failed");

  // Address structure for Server
  memset(&servAddr, 0, sizeof(servAddr));     			// Zero out structure
  servAddr.sin_family      = AF_INET;             	// Internet address family
  servAddr.sin_addr.s_addr = inet_addr(servIP);   	// Server IP address
  servAddr.sin_port        = htons(servPort);     	// Server port

  if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)	// Connect to Server
    DieWithSystemMessage("connect() failed");

	/* GETSOCKNAME */

	getsockname(sock, (struct sockaddr *) &cliAddr, &addrSize);	// Get address and port of local socket
  fprintf(stderr, "Client Address & port:\t%s:%d\n", inet_ntoa(cliAddr.sin_addr), ntohs(cliAddr.sin_port));

	/* SOCKET COMMUNICATION */

	// SEND - Request String
  sprintf(sendStr, "%s\r\n", reqStr);								// Add end-of-line marker to request string
	printf("Sending:\t\t%s\n", reqStr);								// Output the request string

	if (send(sock, sendStr, strlen(sendStr), 0) != strlen(sendStr) || strlen(sendStr) > BUFFSIZE)
    DieWithSystemMessage("send() sent a different number of bytes than expected");
	else if(isspace(sendStr[0]))											// Check if the 1st character is a space
	  DieWithSystemMessage("Syntax error");						// Return a syntax error

  // RECEIVE - Greeting / Error Message From Server
  if ((bytesRcvd = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0)
    DieWithSystemMessage("recv() failed or connection closed prematurely");

  buffer[bytesRcvd] = '\0';  												// Terminate the string!
	printf("Greeting:\t\t%s", buffer);								// Greeting + random text
 
	// Decide if further sending / receiving necessary
	char msgReceived[96];
  sprintf(msgReceived, "%s", buffer);
	char* proceed = strtok (msgReceived, " ,-");

  if(strcmp(msgReceived, "Error") != 0) {
		// SEND - Num of bytes
		sprintf(sendStr, "%d/r/n", bytesRcvd);						// End-of-line terminator
		if (send(sock, sendStr, strlen(sendStr), 0) != strlen(sendStr) || strlen(sendStr) > BUFFSIZE)
		   DieWithSystemMessage("send() sent a different number of bytes than expected");
		else if(isspace(sendStr[0]))											// Syntax check
			 DieWithSystemMessage("Syntax error");

		// RECEIVE - Random num of bytes
		if ((bytesRcvd = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0)
		  DieWithSystemMessage("recv() failed or connection closed prematurely");

		buffer[bytesRcvd] = '\0';  												// Terminate the string!
		printf("Random Bytes:\t\t%s", buffer); 					  // Print the buffer

		int sizeOfBuff = strstr(buffer,"\r\n")-buffer;		// Point in string where searched string "\r\n" begins

		// SEND - Num of bytes
		printf("Random Bytes Amount:\t%d\n", sizeOfBuff);	// size
		sprintf(sendStr, "%d", sizeOfBuff);								// Create a string to send

		if (send(sock, sendStr, strlen(sendStr), 0) != strlen(sendStr) || strlen(sendStr) > BUFFSIZE)
		  DieWithSystemMessage("send() sent a different number of bytes than expected");
		else if(isspace(sendStr[0]))											// Syntax check
			DieWithSystemMessage("Syntax error");

		/* SHUTDOWN SENDING SIDE */
			  
		shutdown(sock, SHUT_WR);													// Shuts down only the sending side, still receives

		// RECEIVE - Outcome string
		if ((bytesRcvd = recv(sock, buffer, BUFFSIZE - 1, 0)) <= 0)
		  DieWithSystemMessage("recv() failed or connection closed prematurely");
		buffer[bytesRcvd] = '\0';  												// Terminate the string!
		printf("Outcome & Cookie:\t%s\n", buffer);    		// Print the  buffer
	}
  close(sock);
  exit(0);
} // end main
