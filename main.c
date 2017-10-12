#include <stdio.h>
#include "tcp_client.h"
//This file demonstrates usage of tcp_client library

int main(void){
    char serverName[] = "d1igiploert.nl";
    int serverPort =  80;
    
    int clientSocket;
    clientSocket = connectToServer(serverName, serverPort);
    if(clientSocket == 0){
        printf("Connect failed with code %d\n", clientSocket);
        return 1;
    }

    int rc;
    rc = sendMessage(&clientSocket, "GET /index.php HTTP/1.1\nhost: www.digiploert.nl\n\n", 1);
    if(rc == 0){
        printf("Sending messsage failed.\n");
        return 1;
    }

    return 0;
}
