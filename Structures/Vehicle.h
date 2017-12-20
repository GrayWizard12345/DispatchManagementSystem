//
// Created by madina on 20/12/17.
//

#pragma once

#include "../global_var/global_var.h"

typedef struct Vehicle Vehicle;

struct Vehicle {
    char number[8];
    char model[MAX_BUFFER];
    char color[MAX_BUFFER];
} initVehicle() {
    printf("Vehicle constructor called!\n");

    Vehicle* vehicle = malloc(sizeof(Vehicle));

    // Do rest of required init here

    Vehicle vehicle1 = *vehicle;
    free(vehicle);
    return vehicle1;
};

void printVehicle(Vehicle vehicle){
    printf("Vehicle number: %s\n", vehicle.number);
    printf("Vehicle model: %s\n", vehicle.model);
    printf("Vehicle color: %s\n", vehicle.color);
}