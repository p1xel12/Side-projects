#include <stdlib.h>

typedef struct {
    int *ptr;
    int len;
} *dynamic_t;

dynamic_t dynamic_init() {
    dynamic_t dyn = (dynamic_t)calloc(1, sizeof(*dyn));
    return dyn;
}

void dynamic_set_size(dynamic_t dyn, size_t size) {
    dyn->len = size;
    dyn->ptr = (int*)realloc(dyn->ptr, dyn->len*sizeof(int));
}

void free_dynamic(dynamic_t dyn) {
    free(dyn->ptr);
    free(dyn);
}