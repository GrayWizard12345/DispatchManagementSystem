//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once
#include "../../Structures/Connection.h"

typedef struct Driver Driver;
struct Driver {
    Connection connection;
    int isUp;
    int id;
};

Connection connectToServer(){
    int sock = 0;
    struct sockaddr_in serv_addr;
    printf("CREATING DRIVER SOCKET .....\n");
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return nullptr;
    }
    printf("DEFINING DRIVER SOCKET FAMILY, ADDRESS & PORT .....\n");
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return nullptr;
    }
    printf("CLIENT CONNECTING ON PORT 8080 TO COMMUNICATE WITH SERVER.....\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return nullptr;
    }

    Connection c = connectionInit();
}