#include <stdio.h>
#include "tcp_client.h"

int main(void){
    char serverName[] = "digiploert.nl";
    int serverPort =  80;
    connectToServer(serverName, serverPort);
    return 0;
}
