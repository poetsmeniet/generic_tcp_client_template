#ifndef tcp_client_H_
#define tcp_client_H_

extern int connectToServer(char *serverName, int serverPort);
extern int sendMessage(int *clientSocket, char *buffer, int reply);

#endif
