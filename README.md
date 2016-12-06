# Network-Services-Applications-CA
Take Home Assignment for 3rd year Netork Services and Applications module of 
Games Design & Development - BSc (Honours) in Computing (Level 8) course. 

Client and Server Sockets in C. Server receives request string from Client 
parses it, and returns a response.

Client socket takes in request string argument when it is run from terminal. 
This request string is sent to the Server socket which parses the string, 
checking that is is formatted correctly. 

It then sends a random number of bytes to the Client, and receives the number 
of bytes the Client received as a response, if the the number of bytes sent 
matches the number of bytes received the Server sends the client a positive 
response, otherwise it sends a negative response. With the response a randomly 
generated cookie is also sent.
