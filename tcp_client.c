#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
#include "tcp_client.h"
#include <arpa/inet.h>
#define MAXSZ 19024

extern int sendMessage(int *clientSocket, char *buffer, int replies, respBuf *responses){
    int rc;
    char buf[MAXSZ];
    memcpy(buf, buffer, strlen(buffer));

    rc = send(*clientSocket,buf,strlen(buf),MSG_NOSIGNAL);
    if(rc != -1)
        printf("sent data: '%s', rc = %d\n", buf, rc);
    else
        return 0;

    //Receive data for nr of expected replies
    int i;
    for(i = 0; i < replies; i++){
        rc = recv(*clientSocket, buf, MAXSZ, MSG_WAITALL);
        if(rc != -1){
            responses[i].nr = i;
            responses[i].buffer = malloc(rc * sizeof(char));
            memcpy(responses[i].buffer, buf, rc);
        }else{
            return 0;
        }
    }
    return 1;
}

extern int connectToServer(char *serverName, int serverPort){
    //Range check for serverPort
    if(serverPort < 1 || serverPort > 65535){
        printf("ServerPort must be of value between 1 and 65535\n");
        return 0;
    }

    int clientSocket;
    
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
    int rc;
    rc = getaddrinfo(serverName, service, &hints, &res);
    if(rc != 0){
        printf("Error code for get address: %d\n", rc);
        printf("\tEAI_AGAIN: %d\n", EAI_AGAIN);
        printf("\tEAI_BADFLAGS: %d\n", EAI_BADFLAGS);
        printf("\tEAI_FAIL: %d\n", EAI_FAIL);
        printf("\tEAI_FAMILY: %d\n", EAI_FAMILY);
        printf("\tEAI_MEMORY: %d\n", EAI_MEMORY);
        printf("\tEAI_NONAME: %d\n", EAI_NONAME);
        printf("\tEAI_SERVICE: %d\n", EAI_SERVICE);
        printf("\tEAI_SOCKTYPE: %d\n", EAI_SOCKTYPE);
        printf("\tEAI_SSYSTEM: %d\n", EAI_SYSTEM);
        return 0;
    }

    printf("getaddrinfo result code: %d\n", rc);

    struct addrinfo *rp;

    //For each address try to connect
    for (rp = res; rp != NULL; rp = rp->ai_next){
        int rc = connect(clientSocket, rp->ai_addr, rp->ai_addrlen);
        if(rc != 0){
          printf("Unable to connect to '%s', rc; %d\n", serverName, rc);
          return 0;
        }
  
        printf("Connected..\n");

        return clientSocket;
    }

    return 0;
}
