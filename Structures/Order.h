//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once
#include "Location.h"

void order_setSource(Order* order, Location source);
void order_setDestination(Order* order, Location destination);

typedef struct Order Order;
struct Order {

    struct Location source;
    struct Location destination;
    char* clientName;
    char* clientPhoneNumber;

    void (*setSource)(void* order, Location source);
    void (*setDestination)(void* order, Location destination);

} orderInit() {
    printf("Order's constructor invoked");
    Order* order = malloc(sizeof(Order));

    // -1 indicates, that order Currently does not belong to any user
    order->userId = -1;

    Order order1 = *order;
    free(order);
    return order1;
};

void order_setSource(Order* order, Location source) {
    Order* o = order;
    o->source = source;
}
void order_setDestination(Order* order, Location destination) {
    Order *o = order;
    o->destination = destination;
}