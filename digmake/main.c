#include <string.h>
#include <math.h>
#include "../alexgl/alexgl.h"
#include "mesh.h"
#include "../alexinput.h"

agl_context_t agl;

mesh_t mesh;

int yRot = 0;

void vertex_shader(agl_context_t ctx, int i, vec2_t *pos) {
    vec3_t apos = ((vec3_t*)agl_get_pointer_data(ctx, 0))[i];

    apos = rot3y(apos, deg_to_rad(yRot));

    *pos = (vec2_t) {
        .x = (apos.x / 2.0f) / (apos.z + 4.0f),
        .y = apos.y / (apos.z + 4.0f)
    };
}

void fragment_shader(agl_context_t ctx, color_t *col) {
    *col = 0xFF0000;
}

int main() {
    agl = agl_start(80, 40);

    vec3_t verts[] = {
        {
            .x = 0.75f,
            .y = -0.75f,
            .z = 0.75f
        },
        {
            .x = 0.75f,
            .y = 0.75f,
            .z = 0.75f
        },
        {
            .x = -0.75f,
            .y = 0.75f,
            .z = 0.75f
        },
        {
            .x = -0.75f,
            .y = -0.75f,
            .z = 0.75f
        },

        {
            .x = -0.75f,
            .y = -0.75f,
            .z = -0.75f
        },
        {
            .x = -0.75f,
            .y = 0.75f,
            .z = -0.75f
        },
        {
            .x = 0.75f,
            .y = 0.75f,
            .z = -0.75f
        },
        {
            .x = 0.75f,
            .y = -0.75f,
            .z = -0.75f
        },

        {
            .x = -0.75f,
            .y = -0.75f,
            .z = 0.75f
        },
        {
            .x = -0.75f,
            .y = 0.75f,
            .z = 0.75f
        },
        {
            .x = -0.75f,
            .y = 0.75f,
            .z = -0.75f
        },
        {
            .x = -0.75f,
            .y = -0.75f,
            .z = -0.75f
        },

        {
            .x = 0.75f,
            .y = -0.75f,
            .z = -0.75f
        },
        {
            .x = 0.75f,
            .y = 0.75f,
            .z = -0.75f
        },
        {
            .x = 0.75f,
            .y = 0.75f,
            .z = 0.75f
        },
        {
            .x = 0.75f,
            .y = -0.75f,
            .z = 0.75f
        }
    };

    int inds[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        8, 9, 10, 10, 11, 8,
        12, 13, 14, 14, 15, 12
    };

    int shader = agl_create_shader(agl, vertex_shader, fragment_shader);
    mesh = mesh_init(agl, verts, 16, inds, 24, shader);

    input_state_t input = start_input();

    while (!input->keyboard['`']) {
        update_input(input);

        if (input->keyboard[ARROW_RIGHT])
            yRot -= 5;
        if (input->keyboard[ARROW_LEFT])
            yRot += 5;

        memset(agl->buf->data, 0, 3200*sizeof(color_t));
        render_mesh(agl, mesh);

        draw_buffer(agl->buf);
    }

    end_input(input);
}