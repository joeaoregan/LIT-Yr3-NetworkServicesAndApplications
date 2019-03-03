/* APPLICATION SETUP AND PARAMETER PARSING */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

  if (argc != 3) // Test for correct number of arguments
    DieWithUserMessage("Parameter(s)", "<Address/Name> <Port/Service>");

  char *addrString = argv[1];   // Server address/name
  char *portString = argv[2];   // Server port/service

/* CONSTRUCT ADDRESS SPECIFICATION */

  // Tell the system what kind(s) of address info we want, declare & initialise addrinfo structure
  struct addrinfo addrCriteria;                   // Criteria for address match
  memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
  addrCriteria.ai_family = AF_UNSPEC;             // Any address family, Family: AF_INET, AF_INET6, AF_UNSPEC
  addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets, Socket type: SOCK_STREAM, SOCK_DGRAM
  addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol, Protocol: 0 (default) or IPPROTO_XXX


/* FETCH ADDRESSES INFORMATION */

  // Get address(es) associated with the specified name/service. Fetch address info
  struct addrinfo *addrList; // Holder for list of addresses returned, pointer to head of linked list
  // Modify servAddr contents to reference linked list of addresses
  
  // getaddrinfo() creates a dynamically allocated linked list of results,
  // must be deallocated after the caller is finished with the list
  int rtnVal = getaddrinfo(addrString, portString, &addrCriteria, &addrList);	// pass hostname, port, constraints, list
  // addrCriteria: filters the results
  if (rtnVal != 0)		// getaddrinfo() returns 0 if successful
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));
    // gai_strerror(): gives a character string describing what went wrong


/* PRINT ADDRESSES */

  // Display returned addresses
  for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next) {	// iterate through linked list
    PrintSocketAddress(addr->ai_addr, stdout);									// print addresses
    fputc('\n', stdout);														// each on a new line
  }


/* FREE ADDRESS LINKED LIST */

  // Frees ass the storage allocated for the list
  freeaddrinfo(addrList); // Free addrinfo allocated in getaddrinfo()
  // Should only be called when the prog. is finished with the returned info.
  // Failure to call this can result in a pernicious memory leak

  exit(0);
}
