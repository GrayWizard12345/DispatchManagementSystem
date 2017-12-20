//
// Created by abdulbosid on 12/19/17.
//

#ifndef SYSADMINVIEW_H
#define SYSADMINVIEW_H

#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "sysadmin.h"
#include "../../global_var/global_var.h"
#include "DriverArray.h"

void addNewDriverView();
void editDriverView();
void allDriversView();
static int myGetLine (char *prmpt, char *buff, size_t sz);

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

    size_t bufferSize = MAX_BUFFER;
    size_t bufferSizeForNumber = 8; //TODO: size of the variable is not defined in the global

    char *number = (char *)malloc(bufferSizeForNumber * sizeof(char));
    char *model = (char *)malloc(bufferSize * sizeof(char));
    char *color = (char *)malloc(bufferSize * sizeof(char));

    password = getpass("  *Please enter the new password: ");
    strcpy(password2, password);
    password = getpass("  *Please confirm the new password: ");

    myGetLine("Please enter the new Driver's vehicle number: ", number, bufferSizeForNumber);
    myGetLine("Please enter the new Driver's vehicle model: ", model, bufferSize);
    myGetLine("Please enter the new Driver's vehicle color: ", color, bufferSize);


    Vehicle vehicle;
    strcpy(vehicle.number, number);
    strcpy(vehicle.model, model);
    strcpy(vehicle.color, color);
//    printf("%d - %s", numberLen, number);
//    if (number[numberLen-1] == '\n')
//        number[numberLen-1] = '\0';
//    if (model[modelLen-1] == '\n')
//        model[modelLen-1] = '\0';
//    if (color[colorLen-1] == '\n')
//        color[colorLen-1] = '\0';

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
        printf("%d.Driver id: %d\n  Vehicle password:%s\n  Vehicle number:%s\n  Vehical model: %s\n  Vehical color: %s\n", i, driverArray.array[i].id, driverArray.array[i].password, driverArray.array[i].vehicle.number, driverArray.array[i].vehicle.model, driverArray.array[i].vehicle.color);
    }
    printf("-----------------------------------------------\n");
}


static int myGetLine (char *prmpt, char *buff, size_t sz) {
    int ch, extra;
    // Get line with buffer overrun protection.
    if (prmpt != NULL) {
        printf ("%s", prmpt);
        fflush (stdout);
    }
    if (fgets (buff, sz, stdin) == NULL)
        return NO_INPUT;

    // If it was too long, there'll be no newline. In that case, we flush
    // to end of line so that excess doesn't affect the next call.
    if (buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF))
            extra = 1;
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Otherwise remove newline and give string back to caller.
    buff[strlen(buff)-1] = '\0';
    return OK;
}

#endif //SYSADMINVIEW_H
