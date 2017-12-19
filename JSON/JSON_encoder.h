//
// Created by madina on 19/12/17.
//

#pragma once

#include <stdio.h>

char* getAuthJSON(char* authMessage, int id, char* password) {
    sprintf(authMessage, "{\"id\": %d, \"password\": \"%s\"}", id, password);
}