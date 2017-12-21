//
// Created by abdulbosid on 12/19/17.
//

#ifndef SYSADMINVIEW_H
#define SYSADMINVIEW_H

/*Enum constants needed in myGetline*/
#define OK       0
#define NO_INPUT 1
#define TOO_LONG 2

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../src/sysadmin/Sysadmin.h"
#include "../global_var/global_var.h"
#include "../Structures/Structures.h"
#include "../Structures/DriverArray.h"

/*Pre-declaration of functions used in this file*/
void addNewDriverView();
void allDriversView();
void editDriverView(int, char, char);
void deleteDriverView();

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
    printf("*    5.Exit                                        *\n");
    printf("*                                                  *\n");
    printf("****************************************************\n");
    printf("Your choice: ");
    do {
        choice = getchar();
        choice -= 48;
        switch (choice)
        {
            case 1:
                system("@cls||clear");
                addNewDriverView();
                return sysadminView(); //Back to main menu
            case 2:
                system("@cls||clear");
                allDriversView();
                printf("Press any key to close...\n");
                getchar();
                getchar();
                system("@cls||clear");
                return sysadminView(); //Back to main menu
            case 3:
                system("@cls||clear");
                editDriverView(0, 0, 0);
                return sysadminView(); //Back to main menu
            case 4:
                system("@cls||clear");
                deleteDriverView();
                return sysadminView(); //Back to main menu
            case 5:
                return;
            default:
                break;
        }
    }while (choice < 1 || choice > 5);
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

    //TODO: Shorten the line somehow
    myGetLine("Please enter the new Driver's vehicle number: ", number, bufferSizeForNumber);
    myGetLine("Please enter the new Driver's vehicle model: ", model, bufferSize);
    myGetLine("Please enter the new Driver's vehicle color: ", color, bufferSize);


    Vehicle vehicle;
    strcpy(vehicle.number, number);
    strcpy(vehicle.model, model);
    strcpy(vehicle.color, color);

    if (strcmp(password, password2) == 0)
    {
        if(0 == addNewDriver(id, password, vehicle))
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


void editDriverView(int choice, char chPass, char chVeh)
{
    DriverArray driverArray = getAllDrivers();
    short changesAreMade = 0;

    allDriversView();// displaying with ordered numbers

    if (choice == 0)
    {
        printf("Choose driver: ");
        scanf("%d", &choice);
        choice--; // choice value is greater to 1 then its index in the array
    }
    if(choice >= 0 && choice < driverArray.used)//validation
    {
        printf("-----------------------------------------------\n");
        printf("   Editing the driver with id: %d\n", driverArray.array[choice].id);
        fflush(stdout);

        /*Reading password*/
        char *password;
        char password2[INPUT_STRING_LENGTH] = {'\0'};

        size_t bufferSize = MAX_BUFFER;
        size_t bufferSizeForNumber = 8; //TODO: size of the variable is not defined in the global

        char *number = (char *)malloc(bufferSizeForNumber * sizeof(char));
        char *model = (char *)malloc(bufferSize * sizeof(char));
        char *color = (char *)malloc(bufferSize * sizeof(char));

        if (chPass != 'y')
        {
            printf("Do you want to change password? (y/n): ");
            getchar();
            scanf("%c", &chPass);
            getchar();
        }
        if (chPass == 'y')
        {
            fflush(stdout);
            password = getpass("  *Please enter the new password: ");
            strcpy(password2, password);
            password = getpass("  *Please confirm the new password: ");

            if (strcmp(password, password2) == 0) {
                strcpy(driverArray.array[choice].password, password2);
                changesAreMade = 1;
            }else
            {
                system("@cls||clear");
                printf("  Passwords didn't match!\n");
                return editDriverView(choice, chPass, chVeh);
            }
        }

        if (chVeh != 'y')
        {
            printf("Do you want to change vehicle info? (y/n): ");

            scanf("%c", &chVeh);
            getchar();
        }
        if (chVeh == 'y')
        {
            //TODO: Shorten the line somehow
            myGetLine("Vehicle number: ", number, bufferSizeForNumber);
            myGetLine("Vehicle model: ", model, bufferSize);
            myGetLine("Vehicle color: ", color, bufferSize);


            strcpy(driverArray.array[choice].vehicle.number, number);
            strcpy(driverArray.array[choice].vehicle.model, model);
            strcpy(driverArray.array[choice].vehicle.color, color);

            changesAreMade = 1;
        }

        if (changesAreMade == 1)
        {
            if(0 == rewriteAllDrivers(&driverArray))
            {
                system("@cls||clear");
                printf("\n\tError accessing database file!\n");
                printf("-----------------------------------------------\n\n");
                return editDriverView(0, 0, 0);
            }
            system("@cls||clear");
            printf("\n\tEdits are saved!\n");
            printf("-----------------------------------------------\n\n");
        }
    } else //input is not valid
    {
        system("@cls||clear");
        printf("---     Invalid input     ---\n");
        return editDriverView(0, 0, 0);
    }

}

void deleteDriverView()
{
    //TODO: deleteDriverView?
}

void allDriversView()
{
    printf("-----------------------------------------------\n");
    printf("-    Drivers registered in the DMSystem       -\n");
    DriverArray driverArray = getAllDrivers();
    for (int i = 0; i < driverArray.used; ++i) {
        printf("%d.Driver id: %d\n  Driver's password:%s\n  Vehicle number:%s\n  Vehical model: %s\n  Vehical color: %s\n", i + 1, driverArray.array[i].id, driverArray.array[i].password, driverArray.array[i].vehicle.number, driverArray.array[i].vehicle.model, driverArray.array[i].vehicle.color); // This line is even longer))
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
    if (fgets (buff, (int) sz, stdin) == NULL)
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
