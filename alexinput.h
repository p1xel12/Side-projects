#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <stdbool.h>
#include <string.h>

#pragma once

enum ALEX_MOUSE_BUTTON {
    ALEX_MOUSE_BUTTON_NONE = -1,
    ALEX_MOUSE_BUTTON_LEFT = 0,
    ALEX_MOUSE_BUTTON_RIGHT = 1,
    ALEX_MOUSE_BUTTON_MIDDLE = 2
};

typedef struct {
    enum ALEX_MOUSE_BUTTON button;
    int x, y, rx, ry;
} mouse_state_t;

typedef struct {
    #define ARROW_UP 78
    #define ARROW_DOWN 79
    #define ARROW_RIGHT 80
    #define ARROW_LEFT 81

    bool keyboard[82];

    mouse_state_t mouse;
} *input_state_t;

static struct termios cooked;
static struct termios raw;

static int mousefd;

input_state_t start_input() {
    tcgetattr(STDIN_FILENO, &cooked);

    cfmakeraw(&raw);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    input_state_t input = (input_state_t)malloc(sizeof(*input));

    input->mouse.x = 0;
    input->mouse.y = 0;

    return input;
}

void update_input(input_state_t input) {
    char c = getchar();

    memset(input->keyboard, false, 82);
    if (c > 47 && c < 127) {
        input->keyboard[c] = true;
    } else if (c == '\033') {
        getchar();
        input->keyboard[getchar()+13] = true;
    }

    
}

void end_input(input_state_t input) {
    free(input);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &cooked);
}