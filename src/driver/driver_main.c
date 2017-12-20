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
#include <time.h>
#include <stdlib.h>

#include "../../Structures/Connection.h"
#include "../../Structures/Driver.h"
#include "../../View/DriverView.h"

Driver* driver;
char sendMessage[MAX_MESSAGE_SIZE];

void startThreadForNotification();
void startThreadForLocationUpdate();
void printMessage(char*);
void notify(void*);
void sendLocation(void*);

int main() {
    Connection c = connectToServer();
    if(c.socket == 0)
        return 0;

    driver = initDriver(&c);
    authDriver(driver);

    printWelcomeMessage();

    startThreadForNotification();
    startThreadForLocationUpdate();

    int choice;
    while (driver->isUp == 1){
        printWaitingMessage();
        //wait until the driver gets order
        while (hasOder(driver) == -1);
        choice = getInputOnArrival(driver);
        if(choice != 1)
            continue;

        notifyOnArrivalAndChangeState(driver);
        getInputOnPickedUp();
        notifyOnPickUpAndChangeState(driver);
        getInputOnArrivalDestination();
        notifyOnArrivalDestination(driver);
    }

    freeDriver(driver);

    return 0;
}

void notify(void *vargp){
    char recMessage[MAX_MESSAGE_SIZE];

    MESSAGE_TYPE message_type;

    while(driver->isUp == 1){
        memset(recMessage, 0, sizeof(recMessage));
        read(driver->connection->socket, recMessage, sizeof(recMessage));
        fflush(stdout);

        message_type = json_getMessageType(recMessage);
        if(message_type == ORDER_GET){
            Order order = json_getOrderFromJson(recMessage);
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
    pthread_create(&tid, NULL, notify, NULL);
    pthread_join(tid, NULL);
}

void startThreadForLocationUpdate(){
    srand(time(NULL));
    pthread_t tid;
    pthread_create(&tid, NULL, sendLocation, NULL);
    pthread_join(tid, NULL);
}

void sendLocation(void *vargp){
    char *mess;
    while (driver->isUp == 1){
        driver->location.longitude = rand() % 181;
        driver->location.latitude = rand() % 91;
        memset(&mess, 0, sizeof(mess));
        mess = json_getJsonStringFromLocation(driver->location);
        send(driver->connection->socket, mess, strlen(mess), 0);
        sleep(60);
    }

    free(mess);
}


