//
// Created by injuly on 12/14/17.
//

#pragma once

#include <stdio.h>
#include "../Structures/Client.h"
#include "../Structures/Driver.h"
#include <stdbool.h>

void clientEnterOrderDetailsView(Client* client) {
    double lon1 = 0, lat1 = 0;
    double lon2 = 0, lat2 = 0;

    char phoneNumber[256];
    char name[256];

    strcpy(phoneNumber, client->phoneNumber);
    strcpy(name, client->name);

    puts("\n-- Enter Order Details --\n");

    puts("\nEnter source location: ");
    puts("\nLongitude: \n");
    scanf("%lf", &lon1);
    puts("\nLatitude: \n");
    scanf("%lf", &lat1);

    puts("\n\nEnter destination location: ");
    puts("\nLongitude: \n");
    scanf("%lf", &lon2);

    puts("\nLatitude: \n");
    scanf("%lf", &lat2);

    Location location1 = locationInit();
    location1.latitude = lat1;
    location1.longitude = lon1;

    Location location2 = locationInit();
    location2.longitude = lon2;
    location2.latitude = lat2;

    Order order = orderInit();
    order.userId = 0;
    order.source = location1;
    order.destination = location2;
    order.clientName = name;
    order.clientPhoneNumber = phoneNumber;

    client->order = order;
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
    char phoneNumber[256];
    char name[256];

    printf("Enter your name: ");
    fgets(name, sizeof(name), stdin);
    printf("Enter phone number: ");
    fgets(phoneNumber, sizeof(phoneNumber), stdin);

    client->phoneNumber = phoneNumber;
    client->name = name;

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

void clientOrderAcceptedView(Vehicle vehicle) {
    puts("\n-- Your Order Accepted! --\n");
    puts("\nYour taxi info:");
    printf("\nCar model: %s\t", vehicle.model);
    printf("\nCar color: %s\t", vehicle.color);
    printf("\nCar number: %s\t", vehicle.number);
}

int clientWaitingForDriverView() {
    int choice = 0;
    puts("\n-- Driver on the way --\n");
}

void clientDriverArrivedView() {
    puts("\n-- Your Driver Arrived and Waiting to Pick You Up --\n");
    puts("\nThanks for using our service!\n");
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