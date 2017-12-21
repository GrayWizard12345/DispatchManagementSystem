//
// Created by Darkhonbek Mamataliev on 15/12/17.
//

#pragma once
#ifndef JSON_PARSER_HEADER
#define JSON_PARSER_HEADER

#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

#include "../Structures/Structures.h"


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

// Order
Order json_getOrderFromJson(char* json_string) {
    puts(json_string);
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

// Vehicle
Vehicle json_getVehicleFromJson(char* json_string) {
    cJSON *root = cJSON_Parse(json_string);
    cJSON *number_item = cJSON_GetObjectItemCaseSensitive(root, "number");
    cJSON *model_item = cJSON_GetObjectItemCaseSensitive(root, "model");
    cJSON *color_item = cJSON_GetObjectItemCaseSensitive(root, "color");

    char* number = "";
    char* model = "";
    char* color = "";

    if (cJSON_IsString(number_item)) {
        number = number_item->valuestring;
    }
    if (cJSON_IsString(model_item)) {
        model = model_item->valuestring;
    }
    if (cJSON_IsString(color_item)) {
        color = color_item->valuestring;
    }

    Vehicle vehicle = initVehicle();
    strcpy(vehicle.number, number);
    strcpy(vehicle.model, model);
    strcpy(vehicle.color, color);

    return vehicle;
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


int json_getMessageType(char* json_string){
    cJSON *root = cJSON_Parse(json_string);
    cJSON *number_item = cJSON_GetObjectItemCaseSensitive(root, "message_type");
    return number_item->valueint;
}

int json_getStateFromJson(char* json_string)
{
    cJSON *root = cJSON_Parse(json_string);
    cJSON *number_item = cJSON_GetObjectItemCaseSensitive(root, "state");
    return number_item->valueint;

}

#endif