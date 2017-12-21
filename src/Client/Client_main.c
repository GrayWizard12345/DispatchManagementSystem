//
// Created by Darkhonbek Mamataliev on 20/12/17.
//

#include <stdio.h>
#include "../../JSON/JSON_parser.h"
#include "../../View/ClientView.h"

Client client;

void startThreadForNotification();
void sendFirstMessage(Client client);

int main() {

    // Connecting to the server
    Connection c = connectToServer();
    if(c.socket == 0) {
        printf("\nClient connection error. Socket = 0. Terminating...\n");
        return 0;
    }
    client = clientInit();
    client.setConnection(&client, &c);
    // Connection successfull if reached here!

    // Sending first byte to the server, to tell that it is a CLIENT
    sendFirstMessage(client);

    // ---- UI starts here ---- //
    // Getting user credentials
    clientEnterPersonalInfoView(&client);

    while(client.isUp == 1) {
        // Main Menu
        int menuChoice = 0;
        do {
            menuChoice = clientMainMenuView(&client);
            if(menuChoice < 1 || menuChoice > 5) {
                printf("\nIncorrect choice\n");
            }
        } while(menuChoice < 1 || menuChoice > 5);

        bool hasOrder = 0;
        switch(menuChoice) {
            case 1:
                clientOrderTaxiView(&client);
                client.orderTaxi;
                break;
            case 2:
                hasOrder = clientCancelOrderView(&client);
                if(hasOrder)
                    client.cancelOrder;
                break;
            case 3:
                clientOrderDetailsView(client);
                break;
            case 4:
                clientPrintPersonalInfoView(client);
                break;
            case 5:
                clientEnterPersonalInfoView(&client);
                break;
        }

    }
    freeClient(&client);
    return 0;
}

void sendFirstMessage(Client client1) {
    char *firstMessage;
    memset(&firstMessage, 0, sizeof(firstMessage));
    firstMessage = json_getJsonStringForFirstMessage(CLIENT);
    send(client1.connection->socket, firstMessage, strlen(firstMessage), 0);
    free(firstMessage);
}