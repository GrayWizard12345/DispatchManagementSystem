// Created by madina on 19/12/17.
//

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"
#include "../Structures/Client.h"
#include "../Structures/Structures.h"


// AuthData
char* json_getJsonStringFromLoginData(int id, char *password) {
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

cJSON* json_addTypeToJson(cJSON* root, char* type) {
    cJSON_AddStringToObject(root, "type", type);
    return root;
}

// Location
char* json_getJsonStringFromLocation(Location location) {
    cJSON* root;
    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "message_type", LOCATION_CHANGE);
    cJSON_AddNumberToObject(root, "longitude", location.longitude);
    cJSON_AddNumberToObject(root, "latitude", location.latitude);

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

    return (char*)cJSON_Print(root);
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
    cJSON_AddNumberToObject(root, "type", ORDER_REQUEST);

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


int json_getIdFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *type_item = cJSON_GetObjectItemCaseSensitive(root, "id");

    int id = type_item->valueint;

    return id;
}

char *json_getPasswordFromJson(char* json_string)
{
    cJSON *root = cJSON_Parse(json_string);
    cJSON *type_item = cJSON_GetObjectItemCaseSensitive(root, "password");

    char* passwd = type_item->valuestring;

    return passwd;
}

char* json_getJsonStringForSimpleMessage(USER_TYPE user_type, MESSAGE_TYPE message_type) {
    cJSON *root;

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "message_type", message_type);
    cJSON_AddNumberToObject(root, "user_type", user_type);

    char* res =  cJSON_Print(root);
    puts(res);
    return res;
}