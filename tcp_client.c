#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include "tcp_client.h"
#include <arpa/inet.h>

// A quick client for a one-off connection
extern int connectToServer(char *serverName, int serverPort){
    //Range check for serverPort
    if(serverPort < 1 && serverPort > 65535){
        printf("ServerPort must be of value between 1 and 65535\n");
        return 1;
    }

    int clientSocket;
    char buffer[1024];
    
    //struct sockaddr_in serverAddr;
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    
    //Get server address from hostname
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Stream socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */
    struct addrinfo *res;

    //Port to connect to
    char service[6];
    sprintf(service, "%d", serverPort);

    //Resulting struct 
    int res1 = getaddrinfo(serverName, service, &hints, &res);
    printf("getaddrinfo result code: %d\n", res1);

    struct addrinfo *rp;

    //For each address try to connect
    for (rp = res; rp != NULL; rp = rp->ai_next){
        int rc = connect(clientSocket, rp->ai_addr, rp->ai_addrlen);
        if(rc != 0){
          printf("Unable to connect to '%s', rc; %d\n", serverName, rc);
          return 1;
        }
  
        printf("Connected..\n");
        
        //Send 
        strcpy(buffer,"QUIT\r\n");
        int rc1 = send(clientSocket,buffer,strlen(buffer),MSG_NOSIGNAL);
        printf("sent data.. rc = %d\n", rc1);

        rc1 = recv(clientSocket, buffer, 1024, 0);
        printf("recv buffer: %s, rc recv: %d\n", buffer, rc1);
    }

    return 0;
}
