#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "Practical.h"

int SetupTCPClientSocket(const char *host, const char *service) {
	
	
  /* RESOLVE THE HOST AND SERVICE */
	
  // Tell the system what kind(s) of address info we want
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // v4 or v6 is OK
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only streaming sockets
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

  // Get address(es)
  struct addrinfo *servAddr; // Holder for returned list of server addrs
  int rtnVal = getaddrinfo(host, service, &addrCriteria, &servAddr);
  if (rtnVal != 0)
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
  
  
  /* ATTEMPT TO CREATE AND CONNECT A SOCKET FROM THE LIST OF ADDRESSES */
  
  int sock = -1;
  for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
    // Create a reliable, stream socket using TCP
    sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);	// return domain and socket type / protocol
    if (sock < 0)	// If the system can't create a socket of the specified type, move onto next address
      continue;     // Socket creation failed; try next address

    // Establish the connection to the echo server
    if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0)	// Attempt to connect to the server
      break;        // Socket connection succeeded; break and return socket

    close(sock);    // Socket connection failed; try next address
    sock = -1;
  }
  
  /* FREE ADDRESS LIST */
  
  freeaddrinfo(servAddr); // Free addrinfo allocated in getaddrinfo(), avoiding memory leaks
  
  /* RETURN RESULTING SOCKET DESCRIPTOR */
  return sock;
}
