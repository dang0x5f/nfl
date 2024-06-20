#include <ncurses.h>

#ifndef _FILE_NODE_H_
#define _FILE_NODE_H_


typedef struct file_node{
    WINDOW* fpad;
    char*   fname;
    char    fperm[11];
}fnode_t;

int  populate_fnodes(fnode_t**, char*);

void draw_fnodes(fnode_t**, int);

void refresh_fnodes(fnode_t**, int, int, int);

void print_fnodes(fnode_t**, int);

void free_fnodes(fnode_t**, int);


#endif
