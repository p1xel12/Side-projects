#include <stdlib.h>
#include <stdbool.h>
#include "../glad/glad.h"
#include "vmath.h"
#include "shader.h"

#pragma once

typedef struct {
    int vao;
    int vbo;
    int ebo;
    shader_t shader;

    vec3_t *verts;
    int *inds;
    int count;
} *mesh_t;

mesh_t mesh_init(vec3_t *verts, size_t vertc, int *inds, size_t indc, shader_t shader) {
    mesh_t mesh = (mesh_t)malloc(sizeof(*mesh));

    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);

    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertc*sizeof(vec3_t), verts, GL_DYNAMIC_DRAW);

    glEnableVertexArrayAttrib(mesh->vao, 0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        false,
        0,
        NULL
    );

    glGenBuffers(1, &mesh->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indc, inds, GL_DYNAMIC_DRAW);

    mesh->count = indc;
    mesh->shader = shader;

    glBindVertexArray(0);
}

void free_mesh(mesh_t mesh) {
    free(mesh);
}

void render_mesh(mesh_t mesh) {
    glBindVertexArray(mesh->vao);

    glUseProgram(mesh->shader.program);
    glDrawElements(GL_TRIANGLES, mesh->count, GL_UNSIGNED_INT, NULL);

    glBindVertexArray(0);
}