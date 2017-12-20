//
// Created by abdulbosid on 12/19/17.
//

#ifndef SYSADMIN_H
#define SYSADMIN_H

#include <stdio.h>
#include <stdlib.h>
#include "../../Structures/Driver.h"
#include "../../Structures/Vehicle.h"
#include "DriverArray.h"

const char *dbPath = "database.db";
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

int addNewDriver(int id, char *password)
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
        fprintf(dbFile, "#%d %s\n", id, password);
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
        char *string;
        sscanf(line, "#%d %s", &foundID, string);

        Driver driver;
        driver.id = foundID;
        strcpy(driver.password, string);

        insertDriverArray(&driverArray, driver);

        position++;
    }
    fclose(dbFileRead);
    if (line)
        free(line);

    return driverArray;

}


#endif //SYSADMIN_H
