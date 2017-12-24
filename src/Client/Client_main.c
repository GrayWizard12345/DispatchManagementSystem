//
// Created by Darkhonbek Mamataliev on 20/12/17.
//

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "../../JSON/JSON_parser.h"
#include "../../View/ClientView.h"

Client client;
pthread_t notifThreadID;

void notify(void*);
void startClientThreadNotification();
void sendFirstMessage(Client client);

int main() {

    // Connecting to the server
    Connection c = connectToServer();
    if(c.socket == 0) {
        printf("\nClient connection error. Socket = 0. Terminating...\n");
        return 0;
    }
    client = clientInit();
    client.isUp = 1;
    client.setConnection(&client, &c);
    // Connection successfull if reached here!

    // Sending first byte to the server, to tell that it is a CLIENT
    sendFirstMessage(client);

    // ---- UI starts here ---- //
    // Getting user credentials

    startClientThreadNotification();

    clientEnterPersonalInfoView(&client);
    clientEnterOrderDetailsView(&client);
    client.orderTaxi(&client);

    while (client.order.userId == -1);

    int orderCancelled = clientWaitingForDriverView();
    if(orderCancelled == 1) {
        client.cancelOrder;
        clientOrderCancelledView();
        client.isUp = 0;
    }

    //while (client.isUp == 1);   //This shit is suspicious
    pthread_join(notifThreadID, NULL);

    return 0;
}

void sendFirstMessage(Client client1) {
    char *firstMessage;
    memset(&firstMessage, 0, sizeof(firstMessage));
    firstMessage = json_getJsonStringForFirstMessage(CLIENT);
    send(client1.connection->socket, firstMessage, strlen(firstMessage), 0);
    free(firstMessage);
}

void startClientThreadNotification() {
    pthread_create(&notifThreadID, NULL, (void *(*)(void *)) notify, NULL);
}

void notify(void *var){
    char recMessage[MAX_MESSAGE_SIZE];

    MESSAGE_TYPE message_type;

    while(true){
        if(client.isUp == 0)
            break;

        memset(recMessage, 0, MAX_MESSAGE_SIZE);
        int bytes = read(client.connection->socket, recMessage, MAX_MESSAGE_SIZE);
        puts(recMessage);
        fflush(stdout);

        if(bytes < 0) {
            puts("\nIncorrect message type from server\n");
            break;
        } else {
            message_type = json_getMessageType(recMessage);
            printf("message typoe: %d", message_type);
            if(message_type == ORDER_ACCEPTED){
                Vehicle vehicle = json_getVehicleFromJson(recMessage);
                clientOrderAcceptedView(vehicle);
            }else if(message_type == DRIVER_ARRIVED){
                clientDriverArrivedView();
            }
        }

    }

    client.isUp = 0;
    //pthread_join(notifThreadID, NULL);
}