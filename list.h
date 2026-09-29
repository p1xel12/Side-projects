#include <stdlib.h>

#pragma once

typedef struct _list_elem *list_elem_t;

struct _list_elem {
    void *val;
    list_elem_t next;
};

typedef struct {
    list_elem_t first;
    int len;
} *list_t;

list_t list_init() {
    list_t list = (list_t)calloc(1, sizeof(*list));
    return list;
}

static list_elem_t list_get_elem_struct(list_t list, int idx) {
    list_elem_t ptr = list->first;

    for (int i = 0; i < idx; i++) {
        ptr = ptr->next;
    }

    return ptr;
}

void *list_get_elem(list_t list, int idx) {
    return list_get_elem_struct(list, idx)->val;
}

static void list_update_len(list_t list) {
    int i = 0;

    for (list_elem_t ptr = list->first; ptr != NULL; ptr = ptr->next)
        i++;
    
    list->len = i;
}

void list_add_elem(list_t list, void *val) {
    list_elem_t elem = (list_elem_t)malloc(sizeof(*elem));
    elem->val = val;

    if (list->first == NULL) {
        list->first = elem;
    } else {
        list_get_elem_struct(list, list->len-1)->next = elem;
    }

    list_update_len(list);
}

void list_remove_elem(list_t list, int idx) {

}

void free_list(list_t list) {
    list_elem_t ptr = list->first;

    while (ptr != NULL) {
        list_elem_t next = ptr->next;
        free(ptr->val);
        free(ptr);
        ptr = next;
    }

    free(list);
}