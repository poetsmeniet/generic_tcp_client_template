#include <stdio.h>
#include "tcp_client.h"
//This file demonstrates usage of tcp_client library

int main(void){
    char serverName[] = "digiploert.nl";
    int serverPort =  22;
    connectToServer(serverName, serverPort);
    return 0;
}
