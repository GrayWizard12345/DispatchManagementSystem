//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#include "Location.h"
#pragma once


typedef struct Order Order;
struct Order {

    struct Location source;
    struct Location destination;
    int userId;

};