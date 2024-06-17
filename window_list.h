#include <ncurses.h>

#ifndef WINDOW_LIST_H
#define WINDOW_LIST_H

typedef struct window_node {
    int winid;
    WINDOW* window;
    struct window_node* next;
} window_node_t;

typedef struct window_list {
    window_node_t* head;
    WINDOW* focus;
    int list_size;
} window_list_t;

window_list_t* create_list(void);

void append_to_list(window_list_t*, WINDOW*);

void free_list(window_list_t*);

#endif
