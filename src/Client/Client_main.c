//
// Created by Darkhonbek Mamataliev on 20/12/17.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "../../Structures/Connection.h"
#include "../../Structures/Client.h"
#include "../../View/ClientView.h"
#include "../../global_var/global_var.h"

Client client;

void startThreadForNotification();

int main() {

    // Connecting to the server
    Connection c = connectToServer();
    if(c.socket == 0) {
        printf("\nClient connection error. Socket = 0. Terminating...\n");
        return 0;
    }
    client = clientInit();
    client.setConnection(&client, c);

    // Connection successfull!




//    startThreadForNotification();


    //freeDriver(driver);

    return 0;
}
//
//void startThreadForNotification(){
//    char recMessage[MAX_MESSAGE_SIZE];
//    int argc;
//    char ** argv;
//
//    while(client->isUp == 1){
//        memset(recMessage, 0, sizeof(recMessage));
//        read(client->connection->socket, recMessage, sizeof(recMessage));
//        if (argc_argv (recMessage, & argc, & argv) != OK) {
//            fprintf (stderr, "Something went wrong.\n");
//            exit (EXIT_FAILURE);
//        }
//        fflush(stdout);
//        printf("%s", argv[0]);
//    }
//}