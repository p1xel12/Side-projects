#include <stdlib.h>

#pragma once

typedef struct _dict_elem *dict_elem_t;

struct _dict_elem {
    void *key;
    void *val;
    dict_elem_t next;
};

typedef struct {
    dict_elem_t first;
    int len;
} *dict_t;

dict_t dict_init() {
    dict_t list = (dict_t)calloc(1, sizeof(*list));

    list->first = NULL;

    return list;
}

static dict_elem_t list_get_elem_struct(dict_t dict, void *key) {
    dict_elem_t ptr = dict->first;

    while (ptr->key != key) {
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
    list_elem_t elem = list_get_elem_struct(list, idx);
    list_get_elem_struct(list, idx-1)->next = elem->next;
    free(elem);

    list_update_len(list);
}

void list_set_elem(list_t list, int idx, void *val) {
    list_get_elem_struct(list, idx)->val = val;
}

void free_list(list_t list) {
    list_elem_t ptr = list->first;

    while (ptr != NULL) {
        list_elem_t next = ptr->next;
        free(ptr);
        ptr = next;
    }

    free(list);
}