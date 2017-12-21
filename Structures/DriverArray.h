//
// Created by abdulbosid on 12/20/17.
//

#ifndef DISPATCHMANAGEMENTSYSTEM_DRIVERARRAY_H
#define DISPATCHMANAGEMENTSYSTEM_DRIVERARRAY_H

#include <glob.h>
#include <stdlib.h>
#include "Driver.h"
#include "Structures.h"


typedef struct DriverArray {
    Driver *array;
    size_t used;
    size_t size;
} DriverArray;

void initDriverArray(DriverArray *a, size_t initialSize) {
    a->array = (Driver *)malloc(initialSize * sizeof(Driver));
    a->used = 0;
    a->size = initialSize;
}

void insertDriverArray(DriverArray *a, Driver element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (Driver *)realloc(a->array, a->size * sizeof(Driver));
    }
    a->array[a->used++] = element;
}

void deleteDriverArray(DriverArray *a, int index) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (index >= 0 && index < a->used) {
        while (index < a->used - 1)
        {
            a->array[index].id = a->array[index + 1].id;
            strcpy(a->array[index].password, a->array[index + 1].password);
            strcpy(a->array[index].vehicle.number, a->array[index + 1].vehicle.number);
            strcpy(a->array[index].vehicle.model, a->array[index + 1].vehicle.model);
            strcpy(a->array[index].vehicle.color, a->array[index + 1].vehicle.color);
            index++;
        }
        a->used--;
    }
}

void freeDriverArray(DriverArray *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}


#endif //DISPATCHMANAGEMENTSYSTEM_DRIVERARRAY_H
