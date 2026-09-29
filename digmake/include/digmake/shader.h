#include <string.h>
#include "../glad/glad.h"

#pragma once

typedef struct {
    int program;
    size_t vertSize;
    size_t fragSize;
} shader_t;

shader_t create_shader(char *vertCode, char *fragCode) {
    shader_t shader;
    shader.vertSize = strlen(vertCode);
    shader.fragSize = strlen(fragCode);

    int vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, vertCode, &shader.vertSize);
    glCompileShader(vertShader);

    int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, fragCode, &shader.fragSize);
    glCompileShader(fragShader);

    shader.program = glCreateProgram();
    glAttachShader(shader.program, vertShader);
    glAttachShader(shader.program, fragShader);
    glLinkProgram(shader.program);

    return shader;
}

shader_t create_shader_from_file(char *vertPth, char *fragPth) {
    FILE *vfile = fopen(vertPth, "r");

    fseek(vfile, 0, SEEK_END);
    int vlen = ftell(vfile);
    fseek(vfile, 0, SEEK_SET);

    char vertCode[vlen];
    fread(vertCode, 1, vlen, vfile);
    fclose(vfile);

    FILE *ffile = fopen(fragPth, "r");

    fseek(ffile, 0, SEEK_END);
    int flen = ftell(ffile);
    fseek(ffile, 0, SEEK_SET);

    char fragCode[flen];
    fread(fragCode, 1, flen, ffile);
    fclose(ffile);

    return create_shader(vertCode, fragCode);
}