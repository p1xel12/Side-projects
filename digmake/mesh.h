#include "../alexgl/alexgl.h"

#pragma once

typedef struct {
    int vid;
    int iid;

    vec3_t *verts;
    int *inds;

    int shader;
} *mesh_t;

mesh_t mesh_init(agl_context_t ctx, vec3_t *verts, int vertc, int *inds, int indc, int shader) {
    mesh_t mesh = (mesh_t)malloc(sizeof(*mesh));

    mesh->verts = verts;
    mesh->inds = inds;

    mesh->vid = agl_create_pointer(ctx, AGL_POINTER_TYPE_VEC3, verts, vertc);
    mesh->iid = agl_create_pointer(ctx, AGL_POINTER_TYPE_INT, inds, indc);

    mesh->shader = shader;

    return mesh;
}

void render_mesh(agl_context_t ctx, mesh_t mesh) {
    ctx->shader = mesh->shader;
    agl_draw_triangles(ctx, mesh->vid, mesh->iid);
}