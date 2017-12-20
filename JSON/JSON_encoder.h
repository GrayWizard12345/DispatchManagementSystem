//
// Created by madina on 19/12/17.
//

#pragma once

#include <stdio.h>
#include "../global_var/enums.h"
#include "cJSON.h"
#include "../Structures/Client.h"
#include "JSON_parser.h"

char* json_getJsonStringFromAuthData(int id, char* password) {
    cJSON *root;

    puts("password: ");
    puts(password);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "id", id);
    cJSON_AddStringToObject(root, "password", password);

    return cJSON_Print(root);
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
    root = json_addTypeToJson(root, "Client");

    return cJSON_Print(root);
}

