/* PROGRAM SETUP AND PARAMETER PARSING */#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[]) {	// Server address/name and string to echo 1st 2 args

  if (argc < 3 || argc > 4) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)",
        "<Server Address/Name> <Echo Word> [<Server Port/Service>]");

  char *server = argv[1];     // First arg: server address/name
  char *echoString = argv[2]; // Second arg: word to echo

  size_t echoStringLen = strlen(echoString);	// Verify the size
  if (echoStringLen > MAXSTRINGLENGTH) 			// Check input length
    DieWithUserMessage(echoString, "string too long");

  // Third arg (optional): server port/service
  char *servPort = (argc == 4) ? argv[3] : "echo";	// Server port OR service name 3rd param -> OR well known service name "echo"

/* GET FOREIGN ADDRESS FOR SERVER */  // Tell the system what kind(s) of address info we want
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family
  // For the following fields, a zero value means "don't care"
  addrCriteria.ai_socktype = SOCK_DGRAM;          // Only datagram sockets
  addrCriteria.ai_protocol = IPPROTO_UDP;         // Only UDP protocol

  // Get address(es)
  struct addrinfo *servAddr; // List of server addresses
  int rtnVal = getaddrinfo(server, servPort, &addrCriteria, &servAddr);	// determine the corresponding address info (family, address, port num)
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

/* SOCKET CREATION AND SETUP */  // Create a datagram/UDP socket
  int sock = socket(servAddr->ai_family, servAddr->ai_socktype,
      servAddr->ai_protocol); // Socket descriptor for client
  if (sock < 0)
    DieWithSystemMessage("socket() failed");

/* SEND A SINGLE ECHO DATAGRAM */  // Send the string to the server
  ssize_t numBytes = sendto(sock, echoString, echoStringLen, 0,
      servAddr->ai_addr, servAddr->ai_addrlen);	// sendto() only needs to know the destination for the datagram, also assigns arbitrary unused port num to the socket, the server uses this port to reply
  if (numBytes < 0)
    DieWithSystemMessage("sendto() failed");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("sendto() error", "sent unexpected number of bytes");

  // Receive a response

/* GET AND PRINT ECHO REPLY */  /* Receive a message */struct sockaddr_storage fromAddr; // Source address of server
  // Set length of from address structure (in-out parameter)
  socklen_t fromAddrLen = sizeof(fromAddr);	// contains the size of the address buffer given by fromAddr // Waits for a UDP packet addressed to this socket's port
  char buffer[MAXSTRINGLENGTH + 1]; // I/O buffer -> copies the data from first datagram, +1 allows null string terminate
  numBytes = recvfrom(sock, buffer, MAXSTRINGLENGTH, 0,
      (struct sockaddr *) &fromAddr, &fromAddrLen);	
  if (numBytes < 0)
    DieWithSystemMessage("recvfrom() failed");
  else if (numBytes != echoStringLen)
    DieWithUserMessage("recvfrom() error", "received unexpected number of bytes");

/* Check message source */  // Verify reception from expected source 
  if (!SockAddrsEqual(servAddr->ai_addr, (struct sockaddr *) &fromAddr)) // fromAddr address passed // Receive a message from any source // Compare socket addresses
    DieWithUserMessage("recvfrom()", "received a packet from unknown source");

  freeaddrinfo(servAddr);	// Not required again so memory is freed

/* Print received string */  buffer[echoStringLen] = '\0';     // Null-terminate received data
  printf("Received: %s\n", buffer); // Print the echoed string

/* WRAP-UP*/  close(sock);	// close the socket
  exit(0);	// exit the program
}
