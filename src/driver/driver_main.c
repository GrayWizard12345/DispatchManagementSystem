//
// Created by madina on 18/12/17.
//

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "../../Structures/Connection.h"
#include "../../Structures/Driver.h"

int main(int argc, char const *argv[]) {
    Connection* c = connectToServer();
    if(c == NULL)
        return 0;

    Driver* driver = initDriver(c);
    authDriver(driver);


    /*char readMessage[1024];
    char sendMessage[1024];
    while(1)
    {
        memset(readMessage, 0, sizeof(readMessage));
        memset(sendMessage, 0, sizeof(sendMessage));
        printf("CLIENT : ");
        fgets (sendMessage, sizeof(sendMessage), stdin);
        send(c->socket , sendMessage , strlen(sendMessage) , 0 );
        read(c->socket , readMessage, 1024);
        printf("%s\n", readMessage );
        sendMessage[strlen(sendMessage)] = '\0';
        if(strncmp(sendMessage, "bye", strlen("bye"))==0)
            break;
    }*/

    freeDriver(driver);

    return 0;
}