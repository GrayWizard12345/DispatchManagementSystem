//
// Created by injuly on 12/14/17.
//

#pragma once

#include <stdio.h>

void clientMainMenuView () {
    int choice = 0;

    printf("\t\tMain Menu\n");
    printf("\t1.Make order\n");
    printf("\t2.Cancel order\n");

    while (choice != 1 && choice != 2) {
        scanf("%d", &choice);
    }

    switch (choice) {

        case 1:
            break;
        case 2:
            break;
        default:
            break;
    }
}

void clientMakeOrderView () {
    clientEnterClientInfoView();
    clientEnterLocationView("source");
    clientEnterLocationView("destination");

}

void clientCancelOrderView() {


}

void clientEnterClientInfoView() {
    char* name;
    char* phoneNumber;
    printf("Enter your name\n");
    scanf("%s", name);
    printf("Enter your phone number\n");
    scanf("%s", phoneNumber);

}

void clientEnterLocationView(char *option ) {
    double latitude;
    double longitude;
    if (option == "source") {
        printf("Enter your coordinates latitude and longitude of your source\n");
        scanf("%lf", &latitude);
        scanf("%lf", &longitude);
    }else {
        printf("Enter your coordinates latitude and longitude of destination\n");
        scanf("%lf", &latitude);
        scanf("%lf", &longitude);
    }
}

void clientOrderAccepted() {
    int choice = 0;

    printf("\t\tYour order is accepted\n");
    printf("Driver info:");
    printf("Car number: %s");
    printf("Car color: %s");
    printf("Car model: %s");
    printf("---------------------------");
    printf("\n1.Cancel order\n");

    while (choice != 1) {
        scanf("%d", &choice);
    }

    // Order canceled, do smth

}
