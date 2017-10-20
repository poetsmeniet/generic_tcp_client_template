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
//This library does not support protocol specifics such as chunk encoding

//Sends data over socket
//returns: 0 on faulure, 1 success
extern int sendMessage(int *clientSocket, char *buffer, unsigned int len){
    int rc;
    char buf[MAXSZ];

    //Check size of buffers
    if(len > MAXSZ){
        printf("Length of buffer exceeds max of %d\n", MAXSZ);
        return 0;
    }

    if(len > 0){
        //Send the buffer as requested by caller
        memcpy(buf, buffer, len);
        rc = send(*clientSocket, buf, len, MSG_NOSIGNAL);
        if(rc != -1){
            //printf("Sent request, rc = %d, len = %d\n", rc, len);
        }else{
            printf("Did not Send request, rc = %d, len = %d\n", rc, len);
            return 0;
        }
    }else{
        //printf("Skipping msg send..\n");
    }

    return 1;
}

/*Receive a message from connected server
 * - Returns nr of messages*/
extern int recvMessage(int *clientSocket, respBuf *responses, size_t replies){
    //Receive data for nr of expected replies (depr this?)
    size_t i;
    int rc;
    char rbuf[MAXSZ];
    size_t replyCnt = 0;

    for(i = 0; i < replies; i++){
        rc = recv(*clientSocket, rbuf, MAXSZ, 0);

        if(rc == -1){
            //do some error handling..
        }else{
            responses[i].nr = i;
            responses[i].buffer = malloc(rc * sizeof(char));
            memcpy(responses[i].buffer, rbuf, rc);
            replyCnt++;
        }
    }

    return replyCnt;
}

//Connects to "server" 
//- returns socket
extern int connectToServer(char *serverName, int serverPort, int sockTimeout){
    //Range check for serverPort
    if(serverPort < 1 || serverPort > 65535){
        printf("ServerPort must be of value between 1 and 65535\n");
        return 0;
    }

    int clientSocket;
    
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    //Set socket timeout
    struct timeval timeout;      
    timeout.tv_sec = sockTimeout;
    timeout.tv_usec = 0;

    if (setsockopt (clientSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        perror("setsockopt failed\n");

    if (setsockopt (clientSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
                sizeof(timeout)) < 0)
        perror("setsockopt failed\n");
    
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

    struct addrinfo *rp;

    //For each address try to connect
    for (rp = res; rp != NULL; rp = rp->ai_next){
        int rc = connect(clientSocket, rp->ai_addr, rp->ai_addrlen);
        if(rc != 0){
          printf("Unable to connect to '%s', rc; %d\n", serverName, rc);
          return 0;
        }
  
        printf("Connected to %s...\n", serverName);

        return clientSocket;
    }

    return 0;
}
