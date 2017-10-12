#include <stdio.h>
#include "tcp_client.h"
//This file demonstrates usage of tcp_client library

int main(void){
    char serverName[] = "digiploert.nl";
    int serverPort =  80;
    int clientSocket = connectToServer(serverName, serverPort);

    sendMessage(&clientSocket, "GET /index.php HTTP/1.1\nhost: www.digiploert.nl\n\n", 1);
    printf("\n\n");
    //sendMessage(&clientSocket, "GET /thoma.php HTTP/1.1\nhost: www.digiploert.nl\n\n", 2);

    return 0;
}
