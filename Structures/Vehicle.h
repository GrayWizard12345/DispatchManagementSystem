//
// Created by madina on 20/12/17.
//

#pragma once

#include "../global_var/global_var.h"

typedef struct Vehicle Vehicle;

struct Vehicle {
    char number[9];
    char model[MAX_BUFFER];
    char color[MAX_BUFFER];
} initVehicle() {

    Vehicle* vehicle = malloc(sizeof(Vehicle));

    // Do rest of required init here

    Vehicle vehicle1 = *vehicle;
    memset(vehicle1.number, 0, sizeof(vehicle1.number));
    memset(vehicle1.model, 0, sizeof(vehicle1.model));
    memset(vehicle1.color, 0, sizeof(vehicle1.color));
    free(vehicle);
    return vehicle1;
};

void printVehicle(Vehicle vehicle){
    fflush(stdout);
    char buffer[MAX_BUFFER];
    printf("Vehicle model: %s\n", vehicle.model);
    printf("Vehicle color: %s\n", vehicle.color);
    printf("Vehicle number: %s\n", vehicle.number);
}