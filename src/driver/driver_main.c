//
// Created by madina on 18/12/17.
//

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include "../../Structures/Connection.h"
#include "../../Structures/Driver.h"
#include "../../View/DriverView.h"
#include "../../global_var/global_var.h"

Driver* driver;
char sendMessage[MAX_MESSAGE_SIZE];

void startThreadForNotification();
void printMessage(char*);

int main(int argc, char const *argv[]) {
    Connection c = connectToServer();
    if(c.socket == 0)
        return 0;

    driver = initDriver(&c);
    authDriver(driver);

    printWelcomeMessage();



    freeDriver(driver);

    return 0;
}

void printMessage(char* message){

}

void startThreadForNotification(){
    char recMessage[MAX_MESSAGE_SIZE];
    while(driver->isUp == 1){
        memset(recMessage, 0, sizeof(recMessage));

    }
}