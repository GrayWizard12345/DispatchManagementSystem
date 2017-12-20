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
