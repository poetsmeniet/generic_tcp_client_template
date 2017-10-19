#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "tcp_client.h"
//This file demonstrates usage of tcp_client library

int main(void){
    //Connect to server
    char serverName[] = "digiploert.nl";
    int serverPort =  80;
    int clientSocket;

    clientSocket = connectToServer(serverName, serverPort, 10);
    if(clientSocket == 0){
        printf("Connect failed with code %d\n", clientSocket);
        return 1;
    }
    
    //Send a message
    int rc;
    char *req = malloc(2000 * sizeof(char));
    memcpy(req, "GET /index.php HTTP/1.0\n\n", 25);

    rc = sendMessage(&clientSocket, req, strlen(req));
    if(rc == 0){
        printf("Sending messsage 1 failed.\n");
        return 1;
    }
    memset(req,0,strlen(req));

    //Get response
    size_t respCnt = 3; //get some chunks
    respBuf *responses = (respBuf*) malloc(respCnt * sizeof(respBuf));
    recvMessage(&clientSocket, responses, respCnt);
    size_t i;
    for(i = 0; i < respCnt; i++)
        printf("%s", responses[i].buffer);
    
    free(responses);
    free(req);
    //closeSocket();

    return 0;
}
