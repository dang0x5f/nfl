#include <stdlib.h>
#include "window_list.h"

window_list_t* create_list(void){
    window_list_t* list = (window_list_t*)malloc(sizeof(window_list_t));

    list->head  = NULL;
    list->focus = NULL;

    list->list_size = 0;

    return list;
}

void append_to_list(window_list_t* list, WINDOW* win){
    list->list_size += 1;

    if(list->head == NULL){
        window_node_t* newnode = (window_node_t*)malloc(sizeof(window_node_t));
        newnode->winid = list->list_size;
        newnode->window = win;
        newnode->next = NULL;

        list->head = newnode;
    } else{
        window_node_t* iter = list->head;
        
        while(iter->next != NULL)
            iter = iter->next;

        window_node_t* newnode = (window_node_t*)malloc(sizeof(window_node_t));
        newnode->winid = list->list_size;
        newnode->window = win;
        newnode->next = NULL;

        iter->next = newnode;
    }
}

void free_list(window_list_t* list){
    if(list == NULL)
        return;

    window_node_t* current = list->head;
    
    if(current == NULL){
        free(list);
        return;
    }

    window_node_t* next = current->next;
    while(current != NULL){
        free(current);
        current = next;
        if(current != NULL)
            next = current->next;
    }
    
    free(list);
}
