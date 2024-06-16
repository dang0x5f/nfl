//TODO: 

#include <stdlib.h>
#include <ncurses.h>
#include "window_list.h"

void init_colors();

void populate_list(window_list_t*);

WINDOW* create_statusbar();

void display_windows(window_list_t*, WINDOW*);

void write_titles(window_list_t*);

void assign_colors(window_list_t*);

void display_winopts(window_list_t*, WINDOW*);

void rotate_colors(window_list_t*);

const char* WIN_TITLES[] = { "CONNECTION", "FILE SELECT", "LOG" };

const char* WINOPTS1[] = { "q=EXIT", "tab=ROTATE", "enter=OK", "c=CONNECT", "d=DISCONNECT", NULL };
const char* WINOPTS2[] = { "q=EXIT", "tab=ROTATE", "enter=OK", "space=SELECT", NULL };
const char* WINOPTS3[] = { "q=EXIT", "tab=ROTATE", "esc=CLEAR", NULL };

int main(void){
    int c, quit = FALSE;
    window_list_t* wins;
    WINDOW* statusbar = NULL;

    initscr();
    init_colors();
    refresh();

    wins = create_list();
    populate_list(wins);
    statusbar = create_statusbar();
    display_windows(wins,statusbar);    

    do {
        noecho();
        c = getch();
        echo();

        switch(c){
            case 'q':
                quit = TRUE;
                break;
            case '\t':
                rotate_colors(wins);                
                display_windows(wins,statusbar);
                break;
            default:
                break;
        }
    }while(!quit);

    free_list(wins);
    endwin();

    return(EXIT_SUCCESS);
}

void init_colors(){
    start_color();
    init_pair(1,COLOR_GREEN,COLOR_BLACK);
    init_pair(2,COLOR_WHITE,COLOR_BLACK);
    init_pair(3,COLOR_BLACK,COLOR_WHITE);
}

void populate_list(window_list_t* list){
    WINDOW *one, *two, *three;

    list->focus = CONNECTION_WINDOWID;

    one = newwin((LINES/5)*3,(COLS/2),0,0);
    wborder(one,0,0,0,0, 0,0,0,0);
    append_to_list(list, one, CONNECTION_WINDOWID);

    two = newwin((LINES/5)*3,(COLS/2),0,COLS - (COLS/2));
    wborder(two,0,0,0,0, 0,0,0,0);
    append_to_list(list, two, FILE_WINDOWID);

    three = newwin(((LINES/5)*2) + (LINES % 5) - 1,COLS,(LINES/5)*3,0);
    wborder(three,0,0,0,0, 0,0,0,0);
    append_to_list(list, three, LOG_WINDOWID);

    write_titles(list);
    assign_colors(list);
}

WINDOW* create_statusbar(){
    WINDOW* sb;
    sb = newwin(1,COLS,LINES - 1,0);
    wbkgd(sb, COLOR_PAIR(3));
    return sb;
}

void display_windows(window_list_t* list, WINDOW* sb){
    window_node_t* iter = list->head;

    while(iter != NULL){
        wrefresh(iter->window);
        iter = iter->next;
    } 

    display_winopts(list, sb);
    
    wrefresh(sb);
}

void write_titles(window_list_t* list){
    window_node_t* iter = list->head;
    int x = 0, indent = 10;

    while(iter != NULL){
        mvwaddstr(iter->window,iter->window->_cury,iter->window->_curx + indent, WIN_TITLES[x]);
        iter = iter->next;
        ++x;
    }
}

void assign_colors(window_list_t* list){
    window_node_t* iter = list->head;

    while(iter != NULL){
        if(list->focus == iter->winid)
            wbkgd(iter->window, COLOR_PAIR(2));
        else
            wbkgd(iter->window, COLOR_PAIR(1));

        iter = iter->next;
    }
}

void display_winopts(window_list_t* list, WINDOW* sb){
    int x = 0, spacing = 5;

    wdeleteln(sb);
    wmove(sb,0,1);
    switch(list->focus){
        case CONNECTION_WINDOWID:
            mvwaddstr(sb, sb->_cury, sb->_curx, WINOPTS1[x]);
            while(WINOPTS1[++x] != NULL)
                mvwaddstr(sb, sb->_cury, sb->_curx + spacing, WINOPTS1[x]);
            break;
        case FILE_WINDOWID:
            mvwaddstr(sb, sb->_cury, 1, WINOPTS2[x]);
            while(WINOPTS2[++x] != NULL)
                mvwaddstr(sb, sb->_cury, sb->_curx + spacing, WINOPTS2[x]);
            break;
        case LOG_WINDOWID:
            mvwaddstr(sb, sb->_cury, 1, WINOPTS3[x]);
            while(WINOPTS3[++x] != NULL)
                mvwaddstr(sb, sb->_cury, sb->_curx + spacing, WINOPTS3[x]);
            break;
    }
}

void rotate_colors(window_list_t* list){
    window_node_t* iter = list->head;

    while(list->focus != iter->winid)
        iter = iter->next;

    wbkgd(iter->window, COLOR_PAIR(1));

    if(iter->next == NULL){
        iter = list->head;
        wbkgd(iter->window, COLOR_PAIR(2));
    } else{
        iter = iter->next;
        wbkgd(iter->window, COLOR_PAIR(2));
    }

    list->focus = iter->winid;
}
