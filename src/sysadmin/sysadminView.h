//
// Created by abdulbosid on 12/19/17.
//

#ifndef UNTITLED3_SYSADMINVIEW_H
#define UNTITLED3_SYSADMINVIEW_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sysadmin.h"

void sysadminView()
{
    int id;
    printf("-----------------------------------------------\n");
    printf("Please enter Driver id: ");
    scanf("%d", &id);
    getchar();
    fflush(stdout);

    /*Reading password*/
    int check;
    char *password;
    char password2[255] = {'\0'};

    password = getpass("*Please enter the new password: ");
    strcpy(password2, password);
    password = getpass("*Please confirm the new password: ");

    if (strcmp(password, password2) == 0)
    {
        if(addNewDriver(id, password) == 0)
        {
            return sysadminView();
        }
    } else
    {
        printf("Passwords didn't match!\n");
        return sysadminView();
    }
    printf("-----------------------------------------------\n");

}


#endif //UNTITLED3_SYSADMINVIEW_H
