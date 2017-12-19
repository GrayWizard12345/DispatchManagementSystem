//
// Created by madina on 19/12/17.
//

#pragma once

typedef enum USER_TYPE USER_TYPE;
typedef enum MESSAGE_TYPE MESSAGE_TYPE;
typedef enum STATE STATE;

enum USER_TYPE{
    CLIENT, DRIVER, SERVER
};

enum MESSAGE_TYPE{
    AUTH, ORDER_GET
};

enum STATE{
    FREE, WAITING, PICKED_UP
};