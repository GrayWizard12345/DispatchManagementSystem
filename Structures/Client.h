//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once
#include "Connection.h"

typedef struct Client Client;
struct Client {

    struct Connection* connection;
    int isUp;
    int id;

};