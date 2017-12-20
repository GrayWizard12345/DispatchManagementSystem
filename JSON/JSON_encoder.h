// Created by madina on 19/12/17.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "../Structures/Client.h"
#include "../Structures/Location.h"
#include "../Structures/Order.h"
#include "../Structures/Driver.h"
#include "../global_var/enums.h"


// AuthData
char* json_getJsonStringFromAuthData(int id, char* password) {
    cJSON *root;

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "message_type", LOG_IN);
    cJSON_AddNumberToObject(root, "id", id);
    cJSON_AddStringToObject(root, "password", password);

    return cJSON_Print(root);
}

char* json_getJsonStringForFirstMessage(USER_TYPE user_type) {
    cJSON *root;

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "message_type", AUTHENTIFICATION);
    cJSON_AddNumberToObject(root, "user_type", user_type);

    return cJSON_Print(root);
}

// Type
// Send by all objects connecting to the server, to tell the server their type
//TODO should be int, not string
char* json_getTypeFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *type_item = cJSON_GetObjectItemCaseSensitive(root, "type");

    char* type = "none";
    if (cJSON_IsString(type_item)) {
        type = type_item->valuestring;
    }

    return type;
}

cJSON* json_addTypeToJson(cJSON* root, char* type) {
    cJSON_AddStringToObject(root, "type", type);
    return root;
}


// Location
char* json_getJsonStringFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "message_type", location.longitude);
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);
    root = json_addTypeToJson(root, "Location");

    return cJSON_Print(root);
}

cJSON* json_getJsonFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);
    root = json_addTypeToJson(root, "Location");

    return root;
}


// Order
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
    root = json_addTypeToJson(root, "Order");

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
    root = json_addTypeToJson(root, "Order");

    return root;
}


// Client
char* json_getJsonStringFromClient(Client client) {
    cJSON *root;
    cJSON *order = json_getJsonFromOrder(client.order);
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", client.id);
    cJSON_AddNumberToObject(root, "isUp", client.isUp);
    cJSON_AddStringToObject(root, "name", client.name);
    cJSON_AddStringToObject(root, "phoneNumber", client.phoneNumber);
    cJSON_AddItemToObject(root, "order", order);
    root = json_addTypeToJson(root, "Client");

    return cJSON_Print(root);
}

// Vehicle
char* json_getJsonStringFromVehicle(Vehicle vehicle) {
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "number", vehicle.number);
    cJSON_AddStringToObject(root, "color", vehicle.color );
    cJSON_AddStringToObject(root, "model", vehicle.model);
    root = json_addTypeToJson(root, "Vehicle");

    return cJSON_Print(root);
}

char* json_getStateChangeMess(STATE state){
    cJSON *root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "message_type", STATE_CHANGE);
    cJSON_AddNumberToObject(root, "state", state);

    return cJSON_Print(root);
}