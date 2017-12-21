//
// Created by injuly on 12/14/17.
//

#pragma once

#include <stdio.h>
#include "../Structures/Client.h"
#include <stdbool.h>

void clientEnterOrderDetailsView(Client* client) {
    double lon1 = 0, lat1 = 0;
    double lon2 = 0, lat2 = 0;

    printf("\n-- Enter Order Details --\n");

    printf("\nEnter source location: ");
    printf("\nLongitude: \n");
    scanf("%lf", &lon1);
    printf("\nLatitude: \n");
    scanf("%lf", &lat1);

    printf("\n\nEnter destination location: ");
    printf("\nLongitude: \n");
    scanf("%lf", &lon2);
    printf("\nLatitude: \n");
    scanf("%lf", &lat2);

    Location location1 = locationInit();
    location1.setLongitude(&location1, lon1);
    location1.setLatitude(&location1, lat1);

    Location location2 = locationInit();
    location2.setLongitude(&location2, lon1);
    location2.setLatitude(&location2, lat1);

    Order order = orderInit();
    order.setSource(&order, location1);
    order.setDestination(&order, location2);

    client->setOrder(client, order);
}

void orderLocationDetailsView(Order order) {
    Location src = order.source;
    Location dest = order.destination;

    printf("\nSource:");
    printf("\nLon: %lf", src.longitude);
    printf("\nLon: %lf", src.latitude);

    printf("\n\nDestination:");
    printf("\nLon: %lf", dest.longitude);
    printf("\nLon: %lf", dest.latitude);

    printf("\nDistance: %lf", src.calculateDistanceTo(&src, &dest));
}

void clientOrderDetailsView(Client client) {
    if (client.orderExists) {
        printf("\n-- Order details --\n");
        printf("\nClient name: %s", client.order.clientName);
        printf("\nClient phone number: %s", client.order.clientPhoneNumber);
        orderLocationDetailsView(client.order);
    } else {
        printf("\nYou have no orders\n");
    }
}

void clientEnterPersonalInfoView(Client* client) {
    printf("\n-- Please Enter Your Personal Data --\n");
    char* phoneNumber;
    char* name;
    memset(&phoneNumber, 0, sizeof(phoneNumber));
    memset(&name, 0, sizeof(name));

    printf("Enter your name: ");
    scanf("%s", name);
    printf("Enter phone number: ");
    scanf("%s", phoneNumber);

    client->setPrivateInformation(client, name, phoneNumber);
    free(phoneNumber);
    free(name);
}

void clientPrintPersonalInfoView(Client client) {
    printf("\n-- Your Personal Data --\n");
    printf("\nName: %s", client.name);
    printf("\nPhone Number: %s", client.phoneNumber);
    clientOrderDetailsView(client);
}

bool clientOrderTaxiView(Client* client) {
    bool check = client->orderExists;
    if(check) {
        printf("\n-- Order In Progress --\n");
    } else {
        clientEnterOrderDetailsView(client);
    }
    return check;
}

bool clientCancelOrderView(Client* client) {
    bool check = client->orderExists;
    if(check) {
        printf("\n-- Cancelling in progress --\n");
    } else {
        printf("\nYou have no order to cancel!");
    }
    return check;
}



int clientMainMenuView(Client* client) {
    int choice = 0;
    printf("\n-- Online Taxi Order --\n");
    printf("\n-- Main Menu --\n");
    printf("\n1. Order Taxi");
    printf("\n2. Cancel Order");
    printf("\n3. View Order Details");
    printf("\n4. View Personal Data");
    printf("\n5. Update Personal Data");

    return choice;
}