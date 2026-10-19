#include <stdio.h>

#pragma once

void error(char *message) {
    printf("\033[1;31mAAC Error:\033[0m %s\n", message);
    exit(1);
}