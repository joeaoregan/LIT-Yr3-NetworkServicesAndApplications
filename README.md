# Network-Services-Applications-CA

<img src="https://lh3.googleusercontent.com/1AV6KmG6uFefbutju65ZbsqK43h-dT_AGYwwPk_j7vwuzI2KIyiDymWQgdOmhpBEUh5sW2ai4OOGaM6Ii5J-R5rNG0S7WKFrh_Lwj6hvsczdSFCcvFePIF25ty5ULFp-RjXOwbs_QVJHwL0WKWY70169xuW1Ui-Tpv1xafOfFe0b9aYQUiJtsZH-bOdlIXwgNFJp8yJ2HciXl1_H5fFm7db38nBhDRJ4GB2uLIr3uLe0vVFXUrMrsXzgBsPfWcqUzUVLD27A8y8lMMqpIuCtYr3IltqHwSITBgtPOe1DXVxzWrFJubNiqZgK-mBCP8iKLfYENt0mobd4U6KzByjFtKy6gc5gmkHob_ETngI9C10N1pg4PHeMqi79GJx8dwjo3H1AIEk-dxquzqaVpt-pGliCN0yGDHuU8KfU21I2jeMMvgtoImS3jcBHCzGLDbFuFvt2vWdL-b-1l0apy4s6iSFEJBcZtbtrtSTMhQMer66jxdn_nuy0khX-5r1bBLITMufIyyx4JuCt90Mu0P8H9bSxU3dF0JszzmD3HQDFVslh8rH4DShFTXFltcIXkT62ro9kOOMFugdJWlHs6MBase6ZppLXbYz4uVfGCjInL1o0d0-HQ44=w678-h795-no" 
alt="Network Services and Applications - CA - Sockets in C" width="426" height="500" border="10" />

<img src="https://lh3.googleusercontent.com/pGeiOgG72UsRwmGhT0V3loiC_Axi65wY8XZTufJxfjVAxUXEe8HaMLGZI600rELrOUwp6UGCUlNR3Qvh8iARBs-nmHQt5ezVmufKBY5IZnQpn_Gp1HQfClmPHw37NtcobEJxHb3leenHXqd5NhGXwZzOdWO5qxyb4CW1eiwV5Xk8HkAmTyN_vZyxnMAEwZ3KDnVnfDCbQa3OkYAb1jtSSABb22ozp98cSLMk59JEldzUOGsiRA-mw5r8_Gcm73igrM1ODn-maejPGEQeBqukUv56VVUOUT0rqHuBlxtm6dMRD7uDwp6E0wIDh0uVegfHXalqXMMX2MBR0mHJAmt1ZtGfGOVs-_SguXPC0BPoJkI_fnCLjfv9GPgX-C9mX-M1l-b6LTwwGFVlnJk627zBlU3AdxFT47JVhlbpSV6-P4LFMFNqQEFURA0mpUEgCPaCWTSfbFNlauBrg6BGx1LqGb5-6_2BxZO-MkKegF_BoS32lOzpCldjgQFO-ihI7-0vnMtXig5Mm3WC2ucwRxZxU3aDEt9Zv37zWgMwkEG-E_Vq0nNnzxbLW5w6_zDV8N0nGbVaCJ3wHU748deujwhwj9G49vK3abnjg0X_McK7E1Np5lEJDpo=w687-h285-no" 
alt="Network Services and Applications - CA - Sockets in C" width="426" height="177" border="10" />

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
