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

const char* WIN_OPTS[4][4] = {
    { "q=EXIT"    , "tab=ROTATE"   , NULL           , NULL },
    { "enter=OK"  , "c=CONNECT"    , "d=DISCONNECT" , NULL },
    { "enter=OK"  , "space=SELECT" , NULL           , NULL },
    { "esc=CLEAR" , NULL           , NULL           , NULL }    };

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
            case 'j':
            case 'k':
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

    one = newwin((LINES/5)*3,(COLS/2),0,0);
    wborder(one,0,0,0,0, 0,0,0,0);
    append_to_list(list, one);

    two = newwin((LINES/5)*3,(COLS/2),0,COLS - (COLS/2));
    wborder(two,0,0,0,0, 0,0,0,0);
    append_to_list(list, two);

    three = newwin(((LINES/5)*2) + (LINES % 5) - 1,COLS,(LINES/5)*3,0);
    wborder(three,0,0,0,0, 0,0,0,0);
    append_to_list(list, three);

    list->focus = list->head;

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
        if(list->focus == iter)
            wbkgd(iter->window, COLOR_PAIR(2));
        else
            wbkgd(iter->window, COLOR_PAIR(1));

        iter = iter->next;
    }
}

void display_winopts(window_list_t* list, WINDOW* sb){
    int spacing = 5, i = 0;

    wdeleteln(sb);
    wmove(sb,0,1);
    
    window_node_t* iter = list->head;
    while(iter != list->focus)
        iter = iter->next;

    while(WIN_OPTS[0][i] != NULL)
        mvwaddstr(sb, sb->_cury, sb->_curx + spacing, WIN_OPTS[0][i++]);

    i = 0;
    while(WIN_OPTS[iter->winid][i] != NULL)
        mvwaddstr(sb, sb->_cury, sb->_curx + spacing, WIN_OPTS[iter->winid][i++]);
}

void rotate_colors(window_list_t* list){
    window_node_t* iter = list->head;

    while(list->focus != iter)
        iter = iter->next;

    wbkgd(iter->window, COLOR_PAIR(1));

    if(iter->next == NULL){
        iter = list->head;
        wbkgd(iter->window, COLOR_PAIR(2));
    } else{
        iter = iter->next;
        wbkgd(iter->window, COLOR_PAIR(2));
    }

    list->focus = iter;
}
