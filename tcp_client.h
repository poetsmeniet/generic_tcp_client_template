#ifndef tcp_client_H_
#define tcp_client_H_

/*Evey call must be preceded by this function to initiate
  a connection to server*/
extern int connectToServer(char *serverName, int serverPort);

/*Sending a message to connected server;
 - Use returned clientsocket from connectToServer
 - char *buffer is the message to be delivered
 - int replies is the number of expected replies from server*/
extern int sendMessage(int *clientSocket, char *buffer, int replies);

#endif
