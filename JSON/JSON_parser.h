//
// Created by Darkhonbek Mamataliev on 15/12/17.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "../Structures/Client.h"
#include "../Structures/Location.h"
#include "../Structures/Order.h"
#include "../Structures/Driver.h"

// Location
Location json_getLocationFromJson(char* json_string) {
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

    Location location = locationInit();
    location.longitude = lon;
    location.latitude = lat;

    return location;
}

char* json_getJsonStringFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);

    return cJSON_Print(root);
}

cJSON* json_getJsonFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);

    return root;
}

// Order
Order json_getOrderFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *source = cJSON_GetObjectItemCaseSensitive(root, "source");
    cJSON *destination = cJSON_GetObjectItemCaseSensitive(root, "destination");
    cJSON *clientName = cJSON_GetObjectItemCaseSensitive(root, "clientName");
    cJSON *clientPhoneNumber = cJSON_GetObjectItemCaseSensitive(root, "clientPhoneNumber");
    cJSON *userId = cJSON_GetObjectItemCaseSensitive(root, "userId");

    Order order = orderInit();
    Location srcLoc = json_getLocationFromJson(cJSON_Print(source));
    Location destLoc = json_getLocationFromJson(cJSON_Print(destination));
    order.source = srcLoc;
    order.destination = destLoc;
    if(cJSON_IsString(clientName)) {
        order.clientName = clientName->valuestring;
    }
    if(cJSON_IsString(clientPhoneNumber)) {
        order.clientPhoneNumber = clientPhoneNumber->valuestring;
    }
    if(cJSON_IsNumber(userId)) {
        order.userId = userId->valuedouble;
    }

    return order;
}

char* json_getJsonStringFromOrder(Order order) {
    cJSON* root;
    cJSON* srcLoc;
    cJSON* destLoc;
    srcLoc = json_getJsonFromLocation(order.source);
    destLoc = json_getJsonFromLocation(order.destination);
    root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "source", srcLoc);
    cJSON_AddItemToObject(root, "destination", destLoc);
    cJSON_AddStringToObject(root, "clientName", order.clientName);
    cJSON_AddStringToObject(root, "clientPhoneNumber", order.clientPhoneNumber);
    cJSON_AddNumberToObject(root, "userId", order.userId);

    return cJSON_Print(root);
}

cJSON* json_getJsonFromOrder(Order order) {
    cJSON* root;
    cJSON* srcLoc;
    cJSON* destLoc;
    srcLoc = json_getJsonFromLocation(order.source);
    destLoc = json_getJsonFromLocation(order.destination);
    root = cJSON_CreateObject();

    cJSON_AddItemToObject(root, "source", srcLoc);
    cJSON_AddItemToObject(root, "destination", destLoc);
    cJSON_AddStringToObject(root, "clientName", order.clientName);
    cJSON_AddStringToObject(root, "clientPhoneNumber", order.clientPhoneNumber);
    cJSON_AddNumberToObject(root, "userId", order.userId);

    return root;
}

// Client
Client json_getClientFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *order_item = cJSON_GetObjectItemCaseSensitive(root, "order");
    cJSON *id_item = cJSON_GetObjectItemCaseSensitive(root, "id");
    cJSON *isUp_item = cJSON_GetObjectItemCaseSensitive(root, "isUp");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON *phoneNumber_item = cJSON_GetObjectItemCaseSensitive(root, "phoneNumber");

    int id = 0;
    int isUp = 0;
    char* name;
    char* phoneNumber;
    Order order = json_getOrderFromJson(cJSON_Print(order_item));

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

    Client client = clientInit();
    client.order = order;
    client.id = id;
    client.isUp = isUp;
    client.name = name;
    client.phoneNumber = phoneNumber;

    return client;
}

char* json_getJsonStringFromClient(Client client) {
    cJSON *root;
    cJSON *order = json_getJsonFromOrder(client.order);
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", client.id);
    cJSON_AddNumberToObject(root, "isUp", client.isUp);
    cJSON_AddStringToObject(root, "name", client.name);
    cJSON_AddStringToObject(root, "phoneNumber", client.phoneNumber);
    cJSON_AddItemToObject(root, "order", order);
    return cJSON_Print(root);
}


// AuthData
char* json_getJsonStringFromAuthData(int id, char* password) {
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", id);
    cJSON_AddStringToObject(root, "password", password);

    return cJSON_Print(root);
}