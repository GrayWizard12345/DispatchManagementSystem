//
// Created by Darkhonbek Mamataliev on 15/12/17.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "Client.h"
#include "Location.h"
#include "Order.h"
#include "Driver.h"

Location* json_GetLocationFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *lon_item = cJSON_GetObjectItemCaseSensitive(root, "longitude");
    cJSON *lat_item = cJSON_GetObjectItemCaseSensitive(root, "latitude");

    double lon = 0, lat = 0;

    if (cJSON_IsNumber(lon_item)) {
        lon = lon_item->valueint;
    }
    if (cJSON_IsNumber(lat_item)) {
        lat = lat_item->valueint;
    }

    Location* location = malloc(sizeof(Location));
    location->longitude = lon;
    location->latitude = lat;

    return location;
}

char* json_getJsonFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);

    return cJSON_Print(root);
}

Client* json_GetClientFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *id_item = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON *isUp_item = cJSON_GetObjectItemCaseSensitive(root, "isUp");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *phoneNumber_item = cJSON_GetObjectItemCaseSensitive(root, "phoneNumber");

    int id = 0;
    int isUp = 0;
    char* name;
    char* phoneNumber;

    if (cJSON_IsNumber(id_item)) {
        id = id_item->valueint;
    }
    if (cJSON_IsNumber(isUp_item)) {
        isUp = isUp_item->valueint;
    }

    if (cJSON_IsString(name_item)) {
        name = name_item->valuestring;
    }
    if (cJSON_IsString(phoneNumber_item)) {
        phoneNumber = phoneNumber_item->valuestring;
    }

    Client* client = malloc(sizeof(Client));
    client->id = id;
    client->isUp = isUp;
    client->name = name;
    client->phoneNumber = phoneNumber;

    return client;
}

char* json_getJsonFromClient(Client client) {
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", client.id);
    cJSON_AddNumberToObject(root, "isUp", client.isUp);
    cJSON_AddStringToObject(root, "name", client.name);
    cJSON_AddStringToObject(root, "phoneNumber", client.phoneNumber);

    return cJSON_Print(root);
}

char* json_getJsonFromAuthData(int id, char* password) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", id);
    cJSON_AddStringToObject(root, "password", password);

    return cJSON_Print(root);
}