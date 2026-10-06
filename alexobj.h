#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "digmake/mesh.h"

#pragma once

mesh_t load_from_obj(agl_context_t ctx, char *pth, int shader) {
    FILE *file = fopen(pth, "r");

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buf[len+1];
    fread(buf, 1, len, file);
    buf[len] = 0;

    vec3_t *verts = (vec3_t*)malloc(0);
    int *inds = (int*)malloc(0);

    int vertc = 0;
    int indc = 0;

    char *nl = "\n";
    char *sp = " ";

    char *linetok;
    char *line = strtok_r(buf, nl, &linetok);

    while (line != NULL) {
        char *wordtok;

        char *word = strtok_r(line, sp, &wordtok);
        while (word != NULL) {
            if (strcmp(word, "v") == 0) {
                vertc++;
                verts = (vec3_t*)realloc(verts, vertc*sizeof(vec3_t));

                verts[vertc-1] = (vec3_t) {
                    .x = (float)atof(strtok_r(NULL, sp, &wordtok)),
                    .y = (float)atof(strtok_r(NULL, sp, &wordtok)),
                    .z = (float)atof(strtok_r(NULL, sp, &wordtok))
                };
            } else if (strcmp(word, "f") == 0) {
                indc += 3;
                inds = (int*)realloc(inds, indc*sizeof(int));

                inds[indc-3] = atoi(strtok_r(NULL, sp, &wordtok))-1;
                inds[indc-2] = atoi(strtok_r(NULL, sp, &wordtok))-1;
                inds[indc-1] = atoi(strtok_r(NULL, sp, &wordtok))-1;
            }

            word = strtok_r(NULL, sp, &wordtok);
        }

        line = strtok_r(NULL, nl, &linetok);
    }

    mesh_t mesh = mesh_init(ctx, verts, vertc, inds, indc, shader);

    return mesh;
}

void free_obj_mesh(agl_context_t ctx, mesh_t mesh) {
    free(mesh->verts);
    free(mesh->inds);
    free_mesh(ctx, mesh);
}