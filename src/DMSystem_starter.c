//
// Created by hamlet on 12/13/17.
//
#include <stdlib.h>
#include <stdio.h>
#include "Structures.h"




int main(int argc, char** argv)
{

    struct Server server = serverInit();

    printf("Hello World\n");


    printf("5 + 4 = %d", server.myMethod(5,4));




    return 0;
}