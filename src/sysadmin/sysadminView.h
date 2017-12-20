//
// Created by abdulbosid on 12/19/17.
//

#ifndef SYSADMINVIEW_H
#define SYSADMINVIEW_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sysadmin.h"
#include "../../global_var/global_var.h"

void addNewDriverView();
void editDriverView();
void allDriversView();

void sysadminView()
{
    int choice;

    printf("********       System administration        ********\n");
    printf("*                                                  *\n");
    printf("*    1.Add new DRIVER                              *\n");
    printf("*    2.All DRIVERs                                 *\n");
    printf("*    3.Edit DRIVER                                 *\n");
    printf("*    4.Delete DRIVER                               *\n");
    printf("*                                                  *\n");
    printf("****************************************************\n");
    printf("Your choice: ");
    do {
        choice = getchar();
        choice -= 48;
        switch (choice)
        {
            case 1:
                system("clear");
                addNewDriverView();
                break;
            case 2:
                system("clear");
                allDriversView();
                break;
            case 3:
                system("clear");
                editDriverView();
                break;
            case 4:
                break;

            default:
                break;
        }
    }while (choice < 1 && choice > 4);


}

void addNewDriverView()
{
    int id;
    printf("-----------------------------------------------\n");
    printf("   Please enter the new Driver's id: ");
    scanf("%d", &id);
    getchar();
    fflush(stdout);

    /*Reading password*/
    int check;
    char *password;
    char password2[INPUT_STRING_LENGTH] = {'\0'};

    password = getpass("  *Please enter the new password: ");
    strcpy(password2, password);
    password = getpass("  *Please confirm the new password: ");

    if (strcmp(password, password2) == 0)
    {
        if(addNewDriver(id, password) == 0)
        {
            return addNewDriverView();
        }
    } else
    {
        printf("  Passwords didn't match!\n");
        return addNewDriverView();
    }
    system("clear");
    printf("\n\tDriver registered successfully!\n");
    printf("-----------------------------------------------\n\n");

    return sysadminView();
}


void editDriverView()
{

}


void allDriversView()
{
    printf("-----------------------------------------------\n");
    printf("-    Drivers registered in the DMSystem       -\n");



}


#endif //SYSADMINVIEW_H
