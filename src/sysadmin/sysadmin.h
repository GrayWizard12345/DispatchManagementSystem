//
// Created by abdulbosid on 12/19/17.
//

#ifndef SYSADMIN_H
#define SYSADMIN_H

#include <stdio.h>
#include <stdlib.h>
#include "../../Structures/Driver.h"
#include "../../Structures/Vehicle.h"
#include "../../Structures/DriverArray.h"

const char *dbPath = "../src/sysadmin/database.db";
FILE *dbFile = NULL;

int checkDriver(int id);

int openDB()
{
    if(dbFile == NULL) {
        dbFile = fopen(dbPath, "a");
        if (dbFile == NULL) {
            printf("Error opening database file...\n");
            exit(5);
        }
    }
}

int addNewDriver(int id, char *password, Vehicle vehicle)
{
    int position = checkDriver(id);
    openDB();
    switch (position)
    {
        case -1:
            printf("DB file error!\n");
            return -1;
        case 0:
            break;
        default:
            printf("Driver with such ID already exists!\n");
            return 0;
    }
    if(dbFile != NULL)
    {
        fprintf(dbFile, "#%d\t%s\t%s\t%s\t%s\n", id, password, vehicle.number, vehicle.model, vehicle.color);
    }
    fclose(dbFile);
    return 1;
}

int checkDriver(int id)
{
    FILE *dbFileRead = fopen(dbPath, "r");
    char *line;
    size_t len = 0;
    ssize_t read;

    if (dbFileRead == NULL)
        return -1;
    int position = 0;
    while ((read = getline(&line, &len, dbFileRead)) != -1) {
        if (read > 2)
        {
            int foundID;
            sscanf(line, "#%d", &foundID);
            position++;
            if (id == foundID)
            {
                fclose(dbFileRead);
                return position;
            }
        }
    }
    fclose(dbFileRead);
    if (line)
        free(line);

    return 0;
}

DriverArray getAllDrivers()
{
    DriverArray driverArray;
    initDriverArray(&driverArray, 5);

    FILE *dbFileRead = fopen(dbPath, "r");
    char *line;
    size_t len = 0;
    ssize_t read;

    if (dbFileRead == NULL)
    {
        printf("Error on opening a file");
        return driverArray;
    }
    int position = 0;
    while ((read = getline(&line, &len, dbFileRead)) != -1) {
        int foundID;
        char password[INPUT_STRING_LENGTH];
        char number[8]; //TODO: size of the variable is not defined in the global
        char model[MAX_BUFFER];
        char color[MAX_BUFFER];
        // getting driver data from the current line
        // order matters
        sscanf(line, "#%d %[^\t] %[^\t] %[^\t] %[^\t]\n", &foundID, password, number, model, color);

        //nothing scary, just replacing \n character if there is any
        if (password[strlen(password)-1] == '\n')
            password[strlen(password)-1] = '\0';
        if (number[strlen(number)-1] == '\n')
            number[strlen(number)-1] = '\0';
        if (model[strlen(model)-1] == '\n')
            model[strlen(model)-1] = '\0';
        if (color[strlen(color)-1] == '\n')
            color[strlen(color)-1] = '\0';


        Driver driver;
        strcpy(driver.vehicle.number, number);
        strcpy(driver.vehicle.model, model);
        strcpy(driver.vehicle.color, color);

        driver.id = foundID;
        strcpy(driver.password, password);

        insertDriverArray(&driverArray, driver);

        position++;
    }
    fclose(dbFileRead);
    if (line)
        free(line);

    return driverArray;

}


#endif //SYSADMIN_H
