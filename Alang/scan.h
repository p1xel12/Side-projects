#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "types.h"
#include "../list.h"

#pragma once

// Increments the stream `buf` and returns the next character.
static char next(char **buf) {
    return *(++(*buf));
}

// Decrements the stream `buf` and returns the previous character.
static char prev(char **buf) {
    return *(--(*buf));
}

// Checks if char `c` is an integer value.
static bool is_int(char c) {
    return c >= '0' && c <= '9';
}

// Sets `token->token` to the next non-whitespace symbol in the stream, and sets appropriate `token->value`. Returns 0 for success, 1 for EOF, -1 for errors.
int scan(char **buf, token_t *token) {
    char c = next(buf);

    while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f') {
        printf("%c\n", c);
        c = next(buf);
    }
    
    switch (c) {
        case 0: {
            return 1;
        }
        case '+': {
            token->token = TK_PLUS;
            break;
        }
        case '-': {
            token->token = TK_MINUS;
            break;
        }
        case '*': {
            token->token = TK_STAR;
            break;
        }
        case '/': {
            token->token = TK_SLASH;
            break;
        }
        default: {
            if (is_int(c)) {
                token->token = TK_INTLIT;
                token->value = 0;

                int i = 0;

                while (is_int(c)) {
                    token->value += (token->value*10) + c - '0';

                    i++;
                    c = next(buf);
                }
                prev(buf);
            } else {
                return -1;
            }

            break;
        }
    }

    return 0;
}

// Scans file `path` and returns its tokens.
token_table_t scan_file(char *pth) {
    printf("scan_file started\n");
    FILE *file = fopen(pth, "r");

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    fseek(file, 0, SEEK_SET);

    char buf[len+1];
    fread(buf, 1, len, file);
    buf[len] = 0;

    fclose(file);

    printf("File read\n");


    token_table_t tab;

    tab.tokens = (token_t*)malloc(0);
    tab.tokenc = 0;

    token_t tok;
    while (scan(&buf, &tok) == 0) {
        printf("Char scanned: %d\n", tok.token);
        tab.tokenc++;
        tab.tokens = (token_t*)realloc(tab.tokens, tab.tokenc * sizeof(token_t));

        tab.tokens[tab.tokenc-1] = tok;
    }

    return tab;
}

// Frees a token table.
void free_token_table(token_table_t tab) {
    free(tab.tokens);
}