#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>
//#include "config.h"
//#include "gpsTools.h"
#include "tcp_client.h"
#include <arpa/inet.h>

// A quick client for a one-off connection
extern int connectToServer(char *serverName, int serverPort){
    int clientSocket;
    char buffer[1024];
    
    struct sockaddr_in serverAddr;
    socklen_t addr_size;
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    

    //Get server address from hostname
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    struct addrinfo *res;
    const char service[3] = "80";

    int res1 = getaddrinfo(serverName, service, &hints, &res);
    printf("getaddrinfo result code: %d\n", res1);

    struct addrinfo *rp;
    for (rp = res; rp != NULL; rp = rp->ai_next){
        printf("we get ip %s from hostname %s\n", rp->ai_addr, serverName);
        
        
        //serverAddr.sin_addr.s_addr = inet_addr(serverAddress);
        //memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
        //addr_size = sizeof serverAddr;
        
        addr_size = sizeof rp;
   
        //int test = connect(clientSocket, (struct sockaddr *) &rp, addr_size);
        int test = connect(clientSocket, rp->ai_addr, rp->ai_addrlen);
        if(test != 0){
          printf("Unable to connect to '%s', rc; %d\n", serverName, test);
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
                                   
                                                          



    //serverAddr.sin_addr.s_addr = inet_addr(serverAddress);
    //memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);  
    //addr_size = sizeof serverAddr;
   
    //if(connect(clientSocket, (struct sockaddr *) &serverAddr, addr_size)){
    //  printf("Unable to connect to '%s'\n", serverName);
    //  return 1;
    //}
  
    //printf("Connected..\n");
    ////Send 
    //strcpy(buffer,"##,imei:369546025829301,A;");
    //if(send(clientSocket,buffer,strlen(buffer),MSG_NOSIGNAL) <= 0)
    //    return 1;
    //recv(clientSocket, buffer, 1024, 0);
  
    //
    ////Exit tracker connection
    ////strcpy(buffer,"exit");
    ////if(send(clientSocket,buffer,strlen(buffer),MSG_NOSIGNAL) <= 0)
    ////    return 1;

    return 0;
}
