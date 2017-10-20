#ifndef tcp_client_H_
#define tcp_client_H_

//Response buffer structure, for storing multiple responses
typedef struct responsesBuffer{
    unsigned int nr;
    char *buffer;
}respBuf;

/*Evey call must be preceded by this function to initiate
  a connection to server*/
extern int connectToServer(char *serverName, int serverPort, int sockTimeout);

/*Sending a message to connected server;
 - Use returned clientsocket from connectToServer
 - char *buffer is the message to be delivered
 - int replies is the number of expected replies from server*/
extern int sendMessage(int *clientSocket, char *buffer, unsigned int len);

/*Receive a message from connected server
 * - Returns nr of messages*/
extern int recvMessage(int *clientSocket, respBuf *responses, size_t replies);

#endif
