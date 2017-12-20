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
#include <pthread.h>

#include "../../Structures/Connection.h"
#include "../../Structures/Driver.h"
#include "../../View/DriverView.h"
#include "../../global_var/global_var.h"
#include "../../JSON/JSON_parser.h"
#include "../../JSON/JSON_encoder.h"

#include "../../utilities/string_splitting.h"

Driver* driver;
char sendMessage[MAX_MESSAGE_SIZE];

void startThreadForNotification();
void printMessage(char*);
void notify(void*);

int main() {
    Connection c = connectToServer();
    if(c.socket == 0)
        return 0;

    driver = initDriver(&c);
    authDriver(driver);

    printWelcomeMessage();

    startThreadForNotification();

    while (driver->isUp == 1){

    }

    freeDriver(driver);

    return 0;
}

void notify(void *vargp){
    char recMessage[MAX_MESSAGE_SIZE];
    int argc;
    char ** argv;

    MESSAGE_TYPE message_type;

    while(driver->isUp == 1){
        memset(recMessage, 0, sizeof(recMessage));
        read(driver->connection->socket, recMessage, sizeof(recMessage));
        if (argc_argv (recMessage, &argc, &argv) != OK) {
            fprintf (stderr, "Something went wrong.\n");
            exit (EXIT_FAILURE);
        }
        fflush(stdout);
        puts(argv[0]);

        message_type = (int) strtol(argv[0], (char **)NULL, 10);
        if(message_type == ORDER_GET){
            //TODO change the initialization order to json parsing
            //Order order = json_getOrderFromJson(recMessage);
            Order order = receivedOrderInit(25);
            setOrderAndChangeState(order, driver);
            printOrderReceivedMessage(driver);
        }else if(message_type == ORDER_CANCEL){
            removeOrderAndChangeState(driver);
            printOrderCancelMessage();
        }
    }
}

void startThreadForNotification(){
    pthread_t tid;
    printf("Before Thread\n");
    //pthread_create(&tid, NULL, notify, NULL);
    //pthread_join(tid, NULL);
}
