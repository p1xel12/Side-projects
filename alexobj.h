#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "digmake/mesh.h"

#pragma once

mesh_t load_from_obj(char *pth) {
    FILE *file = fopen(pth, "r");

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buf[len];
    fread(buf, 1, len, file);

    vec3_t *verts = (vec3_t*)malloc(0);
    int *inds = (int*)malloc(0);

    int vertc = 0;
    int indc = 0;

    char *nl = "\n";
    char *sp = " ";

    char *line = strtok(buf, nl);
    while (line != NULL) {
        char *word = strtok(line, sp);
        while (word != NULL) {
            if (strcmp(word, "v") == 0) {
                vertc++;
                verts = (vec3_t*)realloc()
            }
        }
    }
}