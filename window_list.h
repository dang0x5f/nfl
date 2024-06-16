#include <ncurses.h>

#ifndef WINDOW_LIST_H
#define WINDOW_LIST_H

enum window_id { CONNECTION_WINDOWID = 1, FILE_WINDOWID, LOG_WINDOWID };

typedef struct window_node {
    WINDOW* window;
    int winid;
    struct window_node* next;
} window_node_t;

typedef struct window_list {
    window_node_t* head;
    int focus;
} window_list_t;

window_list_t* create_list(void);

void append_to_list(window_list_t*, WINDOW*, int);

void free_list(window_list_t*);

#endif