//
// Created by Darkhonbek Mamataliev on 13/12/17.
//

#pragma once
#include "Connection.h"

typedef struct Driver Driver;
struct Driver {

    Connection connection;
    int isUp;
    int id;


};