//
// Created by madina on 20/12/17.
//

#pragma once

#include "../global_var/global_var.h"

typedef struct Vehicle Vehicle;
struct Vehicle {
    char number[8];
    char model[INPUT_STRING_LENGTH];
    char color[INPUT_STRING_LENGTH];
} initVehicle() {
    printf("Vehicle constructor called!\n");

    Vehicle* vehicle = malloc(sizeof(Vehicle));

    // Do rest of required init here

    Vehicle vehicle1 = *vehicle;
    free(vehicle);
    return vehicle1;
};