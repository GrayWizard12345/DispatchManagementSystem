//
// Created by madina on 19/12/17.
//

#pragma once

#include <stdio.h>
#include "../global_var/enums.h"

char* getAuthJSON(char* authMessage, int id, char* password) {
    sprintf(authMessage, "%d %d %s", AUTH, id, password);
}

