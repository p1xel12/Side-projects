#include <string.h>
#include "../alexgl/alexgl.h"
#include "mesh.h"
#include "../alexinput.h"
#include "../alexobj.h"

agl_context_t agl;

int xRot = 0;
int yRot = 0;

void vertex(agl_context_t ctx, int i, vec2_t *pos) {
    vec3_t apos = ((vec3_t*)agl_get_pointer_data(ctx, 0))[i];

    apos = rot3y(rot3x(apos, xRot), yRot);

    *pos = (vec2_t) {
        .x = (apos.x / 2.0f) / (apos.z + 4.0f),
        .y = apos.y / (apos.z + 4.0f)
    };
}

void fragment(agl_context_t ctx, color_t *col) {
    *col = 0xFF0000;
}

int main() {
    agl = agl_start(80, 40);

    input_state_t input = start_input();

    vec3_t verts[] = {
        {
            .x = 0.5f,
            .y = -0.5f,
            .z = -0.5f
        },
        {
            .x = 0.5f,
            .y = 0.5f,
            .z = -0.5f
        },
        {
            .x = -0.5f,
            .y = 0.5f,
            .z = -0.5f
        },
        {
            .x = -0.5f,
            .y = -0.5f,
            .z = -0.5f
        }
    };

    int inds[] = {
        0, 1, 2, 2, 3, 0
    };

    int shader = agl_create_shader(agl, vertex, fragment);
    mesh_t mesh = mesh_init(agl, verts, 4, inds, 6, shader);

    while (!input->keyboard['`']) {
        update_input(input);

        memset(agl->buf->data, 0, agl->buf->width*agl->buf->height*sizeof(color_t));
        render_mesh(agl, mesh);

        draw_buffer(agl->buf);
    }

    end_input(input);

    free_mesh(agl, mesh);
    agl_terminate(agl);
}