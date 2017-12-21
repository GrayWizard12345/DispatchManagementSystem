//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once

#include "Location.h"
#include "Order.h"

typedef struct Order Order;

void order_setSource(Order* order, Location source);
void order_setDestination(Order* order, Location destination);

struct Order {

    struct Location source;
    struct Location destination;
    char* clientName;
    char* clientPhoneNumber;
    int userId;


    void (*setSource)(void* order, Location source);
    void (*setDestination)(void* order, Location destination);

} orderInit() {
    //printf("Order's constructor invoked");
    struct Order* order = malloc(sizeof(Order));

    // -1 indicates, that order Currently does not belong to any user
    order->userId = -1;

    struct Order order1 = *order;
    return order1;
};



//TODO should be removed when the problem with JSON will be solved
Order receivedOrderInit(int userID){
    Order order;
    order.userId = userID;
    return order;
}

void order_setSource(Order* order, Location source) {
    Order* o = order;
    o->source = source;
}
void order_setDestination(Order* order, Location destination) {
    Order *o = order;
    o->destination = destination;
}