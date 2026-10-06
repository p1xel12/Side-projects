#include <string.h>
#include "../alexgl/alexgl.h"
#include "mesh.h"
#include "../alexinput.h"
#include "../alexobj.h"

agl_context_t agl;

mesh_t teapot;

int yRot = 0;

void vertex_shader(agl_context_t ctx, int i, vec2_t *pos) {
    vec3_t apos = ((vec3_t*)agl_get_pointer_data(ctx, 0))[i];

    apos = rot3y(apos, deg_to_rad(yRot));

    *pos = (vec2_t) {
        .x = (apos.x / 2.0f) / (apos.z + 4.5f),
        .y = (apos.y - 1.5f) / (apos.z + 4.5f)
    };
}

void fragment_shader(agl_context_t ctx, color_t *col) {
    *col = 0xFF0000;
}

int main() {
    agl = agl_start(80, 40);

    int shader = agl_create_shader(agl, vertex_shader, fragment_shader);
    teapot = load_from_obj(agl, "utah_teapot.obj", shader);

    input_state_t input = start_input();

    while (!input->keyboard['`']) {
        update_input(input);

        if (input->keyboard[ARROW_RIGHT])
            yRot -= 5;
        if (input->keyboard[ARROW_LEFT])
            yRot += 5;

        memset(agl->buf->data, 0, agl->buf->width*agl->buf->height*sizeof(color_t));
        render_mesh(agl, teapot);

        draw_buffer(agl->buf);
    }

    free_obj_mesh(agl, teapot);
    end_input(input);
}