#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include "../include/glad/glad.h"
#include "../include/digmake/mesh.h"

#define IDX(x, y, w) ((x) + (y) * (w))

typedef unsigned int color_t;

typedef struct _game *game_t;
typedef void (*game_tick_func)(game_t);

struct _game {
    EGLDisplay dpy;
    EGLConfig cfg;
    EGLSurface surf;
    EGLContext ctx;
    
    int width, height;
    color_t *data;

    game_tick_func tick;
};

game_t game_init(int width, int height) {
    game_t game = (game_t)malloc(sizeof(*game));

    game->width = width;
    game->height = height;

    game->dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(game->dpy, NULL, NULL);

    int numCfgs;
    eglChooseConfig(game->dpy, NULL, &game->cfg, 1, &numCfgs);

    int options[] = {
        EGL_WIDTH, width,
        EGL_HEIGHT, height,
        EGL_NONE
    };
    game->surf = eglCreatePbufferSurface(game->dpy, game->cfg, options);

    game->ctx = eglCreateContext(game->dpy, game->cfg, NULL, NULL);
    eglMakeCurrent(game->dpy, game->surf, game->surf, game->ctx);

    gladLoadGLLoader((GLADloadproc)eglGetProcAddress);

    return game;
}

void destroy_game(game_t game) {
    eglDestroyContext(game->dpy, game->ctx);
    eglDestroySurface(game->dpy, game->surf);
    eglTerminate(game->dpy);

    free(game);
}

void start_game(game_t game) {
    while (1) {
        game->tick(game);
        eglSwapBuffers(game->dpy, game->surf);

        glReadPixels(0, 0, game->width, game->height, GL_RGB, GL_UNSIGNED_INT, game->data);

        printf("\033[H");
        for (int x = 0; x < game->width; x++) {
            for (int y = 0; y < game->height; y += 2) {
                color_t p0 = game->data[IDX(x, y, game->width)];
                color_t p1 = game->data[IDX(x, y+1, game->width)];

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
            printf("\033[0m\n");
        }
    }
}

shader_t red;
mesh_t tri;

void tick(game_t game) {
    render_mesh(tri);
}

int main() {
    game_t digmake = game_init(100, 100);

    red = create_shader_from_file("red.vsh", "red.fsh");

    vec3_t verts[] = {
        {
            .x = -0.5f,
            .y = -0.5f,
            .z = -1.0f
        },
        {
            .x = -0.5f,
            .y = 0.5f,
            .z = -1.0f
        },
        {
            .x = 0.5f,
            .y = 0.5f,
            .z = -1.0f
        },
        {
            .x = 0.5f,
            .y = -0.5f,
            .z = -1.0f
        }
    };
    int inds[] = {
        0, 1, 2, 2, 3, 0
    };

    tri = mesh_init(verts, 4, inds, 6, red);

    digmake->tick = tick;
    start_game(digmake);

    destroy_game(digmake);
    return 0;
}