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
#include "DriverArray.h"

void addNewDriverView();
void editDriverView();
void allDriversView();

void sysadminView()
{
    int choice = 0;

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
                sysadminView();
                break;
            case 2:
                system("clear");
                allDriversView();
                return sysadminView();
            case 3:
                system("clear");
                editDriverView();
                return sysadminView();
            case 4:
                break;

            default:
                break;
        }
    }while (choice < 1 || choice > 4);


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
    char *password;
    char password2[INPUT_STRING_LENGTH] = {'\0'};
    Vehicle vehicle;

    char number[8]; //TODO: size of the variable is not defined in the global
    char model[MAX_BUFFER];
    char color[MAX_BUFFER];

    password = getpass("  *Please enter the new password: ");
    strcpy(password2, password);
    password = getpass("  *Please confirm the new password: ");

    printf("   Please enter the new Driver's vehicle number:\n");
    getline(number, (sizeof(char)*8), stdin);
    printf("   Please enter the new Driver's vehicle model:\n");
    getline(model, (sizeof(char)*MAX_BUFFER), stdin);
    printf("   Please enter the new Driver's vehicle color:\n");
    getline(color, (sizeof(char)*MAX_BUFFER), stdin);


    if (strcmp(password, password2) == 0)
    {
        if(addNewDriver(id, password, vehicle) == 0)
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
}


void editDriverView()
{

}


void allDriversView()
{
    printf("-----------------------------------------------\n");
    printf("-    Drivers registered in the DMSystem       -\n");
    DriverArray driverArray = getAllDrivers();
    for (int i = 0; i < driverArray.used; ++i) {
        printf("%d.Driver id: %d\n  Vehicle number:%s\n  Vehical model: %s\n  Vehical color: %s\n", i, driverArray.array[i].id, driverArray.array[i].vehicle.number, driverArray.array[i].vehicle.model, driverArray.array[i].vehicle.color);
    }
    printf("-----------------------------------------------\n");
}


#endif //SYSADMINVIEW_H
