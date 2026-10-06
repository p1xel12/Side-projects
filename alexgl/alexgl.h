#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vmath.h"
#include "../list.h"

#ifndef _AGL_ALEXGL_H
#define _AGL_ALEXGL_H

#define IDX(x, y, w) ((x) + (y) * (w))

#define AGLAPI(name, ret, ...) typedef ret (*name ## _func)(agl_context_t __VA_OPT__(,)__VA_ARGS__); static ret agl_ ## name(agl_context_t ctx __VA_OPT__(,)__VA_ARGS__)
#define AGLEXT static

typedef unsigned int color_t;

typedef struct {
    int width, height;
    color_t *data;
} *buffer_t;

typedef struct {
    buffer_t buf;

    list_t constants;
    list_t pointers;

    list_t shaders;
    int shader;

    /*create_pointer_func create_pointer;
    get_pointer_data_func get_pointer_data;
    set_pointer_data_func set_pointer_data;

    create_shader_func create_shader;

    draw_triangles_func draw_triangles;*/
} *agl_context_t;


AGLAPI(create_constant, int, void *val) {
    list_add_elem(ctx->constants, val);
    return ctx->constants->len-1;
}

AGLAPI(get_constant, void *, int idx) {
    return list_get_elem(ctx->constants, idx);
}

AGLAPI(set_constant, void, int idx, void *val) {
    list_set_elem(ctx->constants, idx, val);
}

enum AGL_POINTER_TYPE {
    AGL_POINTER_TYPE_CHAR = 0,
    AGL_POINTER_TYPE_SHORT = 1,
    AGL_POINTER_TYPE_INT = 2,
    AGL_POINTER_TYPE_FLOAT = 3,
    AGL_POINTER_TYPE_VEC2 = 4,
    AGL_POINTER_TYPE_VEC3 = 5,
    AGL_POINTER_TYPE_VEC4 = 6
};

typedef struct {
    enum AGL_POINTER_TYPE type;

    void *data;
    size_t len;
} *agl_pointer_t;

AGLAPI(create_pointer, int, enum AGL_POINTER_TYPE type, void *data, size_t len) {
    agl_pointer_t ptr = (agl_pointer_t)malloc(sizeof(*ptr));

    ptr->type = type;
    ptr->data = data;
    ptr->len = len;

    list_add_elem(ctx->pointers, ptr);
    return ctx->pointers->len-1;
}

AGLAPI(destroy_pointer, void, int idx) {
    free(list_get_elem(ctx->pointers, idx));
    list_remove_elem(ctx->pointers, idx);
}

AGLAPI(get_pointer_data, void *, int idx) {
    return ((agl_pointer_t)list_get_elem(ctx->pointers, idx))->data;
}

AGLAPI(set_pointer_data, void, int idx, void *data) {
    ((agl_pointer_t)list_get_elem(ctx->pointers, idx))->data = data;
}

typedef void (*vertex_shader_func)(agl_context_t ctx, int i, vec2_t *vertex);
typedef void (*fragment_shader_func)(agl_context_t ctx, color_t *color);

typedef struct {
    vertex_shader_func vertex;
    fragment_shader_func fragment;
} *agl_shader_t;

AGLAPI(create_shader, int, vertex_shader_func vertex, fragment_shader_func fragment) {
    agl_shader_t shader = (agl_shader_t)malloc(sizeof(*shader));

    shader->vertex = vertex;
    shader->fragment = fragment;

    list_add_elem(ctx->shaders, shader);
    return ctx->shaders->len-1;
}

AGLEXT bool linedot2(vec2_t line[2], vec2_t point) {
    vec2_t pa = sub2(point, line[0]);
    vec2_t ba = sub2(line[1], line[0]);
    return cross2(ba, pa) >= 0;
}

AGLAPI(draw_triangles, void, int vertPtrId, int indPtrId) {
    agl_pointer_t vertPtr = list_get_elem(ctx->pointers, vertPtrId);
    agl_pointer_t indPtr = list_get_elem(ctx->pointers, indPtrId);

    agl_shader_t shader = list_get_elem(ctx->shaders, ctx->shader);

    vec2_t verts[vertPtr->len];
    for (int i = 0; i < vertPtr->len; i++) {
        shader->vertex(ctx, i, verts+i);
    }

    for (int i = 0; i < indPtr->len; i += 3) {
        vec2_t v0 = verts[((int*)indPtr->data)[i]];
        vec2_t v1 = verts[((int*)indPtr->data)[i+1]]; // MACHINE. TURN BACK NOW.
        vec2_t v2 = verts[((int*)indPtr->data)[i+2]];

        v0 = (vec2_t) {
            .x = (v0.x + 1.0f) * 0.5f * (float)ctx->buf->width,
            .y = (1.0f - v0.y) * 0.5f * (float)ctx->buf->height
        };
        v1 = (vec2_t) {
            .x = (v1.x + 1.0f) * 0.5f * (float)ctx->buf->width,
            .y = (1.0f - v1.y) * 0.5f * (float)ctx->buf->height
        };
        v2 = (vec2_t) {
            .x = (v2.x + 1.0f) * 0.5f * (float)ctx->buf->width,
            .y = (1.0f - v2.y) * 0.5f * (float)ctx->buf->height
        };

        vec2_t l0[2] = {
            v0, v1
        };
        vec2_t l1[2] = {
            v1, v2
        };
        vec2_t l2[2] = {
            v2, v0
        };

        for (int x = 0; x < ctx->buf->width; x++) {
            for (int y = 0; y < ctx->buf->height; y++) {
                vec2_t p = {
                    .x = x,
                    .y = y
                };

                if (linedot2(l0, p) && linedot2(l1, p) && linedot2(l2, p)) {
                    shader->fragment(ctx, &ctx->buf->data[IDX(x, y, ctx->buf->width)]);
                }
            }
        }
    }
}

buffer_t buffer_init(int width, int height) {
    buffer_t buffer = (buffer_t)malloc(sizeof(*buffer));

    buffer->width = width;
    buffer->height = height;

    buffer->data = (color_t*)malloc(width*height*sizeof(color_t));

    return buffer;
}

void destroy_buffer(buffer_t buffer) {
    free(buffer->data);
    free(buffer);
}

void draw_buffer(buffer_t buffer) {
    printf("\033[H");

    for (int y = 0; y < buffer->height; y += 2) {
        for (int x = 0; x < buffer->width; x++) {
            color_t p0 = buffer->data[IDX(x, y, buffer->width)];
            color_t p1 = buffer->data[IDX(x, y+1, buffer->width)];

            printf(
                "\033[48;2;%d;%d;%d;38;2;%d;%d;%dm▄\033[0m",
                p0 >> 16 & 0xFF,
                p0 >> 8 & 0xFF,
                p0 & 0xFF,
                p1 >> 16 & 0xFF,
                p1 >> 8 & 0xFF,
                p1 & 0xFF
            );
        }
        printf("\033[0m\r\n");
    }
}

agl_context_t agl_start(int width, int height) {
    agl_context_t ctx = (agl_context_t)malloc(sizeof(*ctx));

    ctx->buf = buffer_init(width, height);

    ctx->pointers = list_init();
    ctx->shaders = list_init();
    ctx->shader = 0;

    /*ctx->create_pointer = agl_create_pointer;
    ctx->get_pointer_data = agl_get_pointer_data;
    ctx->set_pointer_data = agl_set_pointer_data;

    ctx->create_shader = agl_create_shader;

    ctx->draw_triangles = agl_draw_triangles;*/

    return ctx;
}

void agl_terminate(agl_context_t ctx) {
    destroy_buffer(ctx->buf);

    for (int i = 0; i < ctx->pointers->len; i++) {
        free(list_get_elem(ctx->pointers, i));
    }

    free_list(ctx->pointers);

    for (int i = 0; i < ctx->shaders->len; i++) {
        free(list_get_elem(ctx->shaders, i));
    }

    free_list(ctx->shaders);
}

#endif