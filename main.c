#include <stdio.h>
#include <stdlib.h>
#include "tcp_client.h"
//This file demonstrates usage of tcp_client library

int main(void){
    //Connect to server
    char serverName[] = "digiploert.nl";
    int serverPort =  80;
    int clientSocket;

    clientSocket = connectToServer(serverName, serverPort);
    if(clientSocket == 0){
        printf("Connect failed with code %d\n", clientSocket);
        return 1;
    }

    //Send a message and get response(s)
    int rc;
    int respCnt = 1;
    respBuf *responses = (respBuf*) malloc(respCnt * sizeof(respBuf));
    rc = sendMessage(&clientSocket, "GET /test.html HTTP/1.1\nhost: digiploert.nl\nConnection: Keep-Alive\n\n", 68,\
            respCnt, responses);
    if(rc == 0){
        printf("Sending messsage failed.\n");
        return 1;
    }
    //Print response(s)
    int i;
    for(i = 0; i < respCnt; i++){
        printf("\n*********** response Nr %d:\n'%s'\n\n",responses[i].nr , responses[i].buffer);
    }
    
    //free(responses);

    //closeSocket();

    return 0;
}
