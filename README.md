# Network-Services-Applications-CA

<img src="https://lh3.googleusercontent.com/1AV6KmG6uFefbutju65ZbsqK43h-dT_AGYwwPk_j7vwuzI2KIyiDymWQgdOmhpBEUh5sW2ai4OOGaM6Ii5J-R5rNG0S7WKFrh_Lwj6hvsczdSFCcvFePIF25ty5ULFp-RjXOwbs_QVJHwL0WKWY70169xuW1Ui-Tpv1xafOfFe0b9aYQUiJtsZH-bOdlIXwgNFJp8yJ2HciXl1_H5fFm7db38nBhDRJ4GB2uLIr3uLe0vVFXUrMrsXzgBsPfWcqUzUVLD27A8y8lMMqpIuCtYr3IltqHwSITBgtPOe1DXVxzWrFJubNiqZgK-mBCP8iKLfYENt0mobd4U6KzByjFtKy6gc5gmkHob_ETngI9C10N1pg4PHeMqi79GJx8dwjo3H1AIEk-dxquzqaVpt-pGliCN0yGDHuU8KfU21I2jeMMvgtoImS3jcBHCzGLDbFuFvt2vWdL-b-1l0apy4s6iSFEJBcZtbtrtSTMhQMer66jxdn_nuy0khX-5r1bBLITMufIyyx4JuCt90Mu0P8H9bSxU3dF0JszzmD3HQDFVslh8rH4DShFTXFltcIXkT62ro9kOOMFugdJWlHs6MBase6ZppLXbYz4uVfGCjInL1o0d0-HQ44=w678-h795-no" 
alt="Network Services and Applications - CA - Sockets in C" width="426" height="500" border="10" />

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
