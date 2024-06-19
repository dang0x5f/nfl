#include <stdlib.h>
#include <ncurses.h>

#include "file_nodes.h"

#define PWD "./"
/* #define SIZE 15 */
/* #define SIZE 48 */
#define SIZE 250

void init(void);
void loop(void);

void create_allpads(WINDOW**);
void refresh_allpads(WINDOW**);
void refresh_pads_range(WINDOW**,int,int,int);

void start_here(WINDOW*);

void move_cursor(int, fnode_t**, int);
void no_scroll(int, fnode_t**, int);
void with_scroll(int, fnode_t**, int);

static int maxy, maxx;
static int y, x;

static int index;

int main(void){

    init();

    loop();

    endwin();

    return(EXIT_SUCCESS);
}

void init(void){
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    curs_set(0);
    refresh();
}

void loop(void){
    int key, quit = FALSE;

    getmaxyx(stdscr, maxy, maxx);

    fnode_t* filepads;    
    int f_cnt = populate_fnodes(&filepads);
    draw_fnodes(&filepads, f_cnt);

    do {
        noecho();
        key = getch();
        echo();

        switch(key){
            case 'q':
                quit = TRUE;
                break;
            case 'k':
            case 'j':
            case 'h':
            case 'l':
                move_cursor(key, &filepads, f_cnt);
                break;
            default:
                break;
        }

    } while(!quit);

    free_fnodes(&filepads, f_cnt);
}

void create_allpads(WINDOW** pads){
    for(int i = 0; i < SIZE; i++){
        pads[i] = newpad(1, maxx);
        mvwprintw(pads[i], 0, 0, "%d)", i);
    }

    start_here(pads[0]);
}

void refresh_allpads(WINDOW** pads){
    refresh();
    for (int i = 0; i < SIZE; i++)
        prefresh(pads[i], 0,0, i,0, i+1,maxx);
}

void refresh_pads_range(WINDOW** pads, int start, int length, int dir){

    if (dir == 0){
        wbkgd(pads[index + 1], A_NORMAL);
        wbkgd(pads[index], COLOR_PAIR(1));
    }
    else if(dir == 1){
        wbkgd(pads[index + length - 1], A_NORMAL);
        wbkgd(pads[index + length], COLOR_PAIR(1));
    }

    refresh();
    for (int i = 0, j = start; i < maxy; i++, j++)
        prefresh(pads[j], 0,0, i,0, i+1,maxx);
}

void start_here(WINDOW* pad){
    wbkgd(pad, COLOR_PAIR(1));
    move(0,0);
}

void move_cursor(int key, fnode_t** pads, int f_cnt){
    getyx(stdscr, y, x);

    if(f_cnt <= maxy - 1){
        no_scroll(key, pads, f_cnt);
    }

            // debug //
    /* wbkgd((*pads)[1].fpad, COLOR_PAIR(1)); */
    /* prefresh((*pads)[1].fpad, 0,0, 1,0,  2,getmaxx(stdscr)); */

    /* if(SIZE <= maxy - 1) */
    /*     no_scroll(key, pads); */
    /* else if(SIZE > maxy - 1) */
    /*     with_scroll(key, pads); */
}

void no_scroll(int key, fnode_t** pads, int f_cnt){
            // debug //
    /* wbkgd((*pads)[1].fpad, COLOR_PAIR(1)); */
    /* prefresh((*pads)[1].fpad, 0,0, 1,0,  2,getmaxx(stdscr)); */

    if ( (key == 'k') && (y > 0) ){
        move(y - 1, x);
        wbkgd((*pads)[y - 1].fpad, COLOR_PAIR(1));
        wbkgd((*pads)[y].fpad, A_NORMAL);

        prefresh((*pads)[y - 1].fpad,  0,0,  y-1,0,  y,maxx);
        prefresh((*pads)[y].fpad,      0,0,  y,0,    y+1,maxx);
    }
    else if( (key == 'j') && (y < f_cnt - 1) ){
        move(y + 1, x);
        wbkgd((*pads)[y + 1].fpad, COLOR_PAIR(1));
        wbkgd((*pads)[y].fpad,     A_NORMAL);

        prefresh((*pads)[y + 1].fpad,  0,0,   y+1,0,  y+2,maxx);
        prefresh((*pads)[y].fpad,      0,0,   y,0,    y+1,maxx);
    }
}

void with_scroll(int key, fnode_t** pads, int f_cnt){
    if( (key == 'k') && (y > 0) ){
        move(y - 1, x);
        wbkgd(pads[y + index-1], COLOR_PAIR(1));
        wbkgd(pads[y +index], A_NORMAL);

        prefresh(pads[y+index-1],  0,0,  y-1,0,  y,maxx);
        prefresh(pads[y+index],    0,0,  y,0,    y+1,maxx);
    }
    else if( (key == 'k') && (y == 0) ){
        ( (--index) < 0 ) ? index = 0 : index;
        if( (index) >= 0 )
            refresh_pads_range(pads, index, maxy - 1, 0);
        move(0,0);
    }
    else if( (key == 'j') && (y < maxy - 1) ){
        move(y + 1, x);
        wbkgd(pads[y +index+ 1], COLOR_PAIR(1));
        wbkgd(pads[y +index], A_NORMAL);

        prefresh(pads[y+index+1], 0,0, y+1,0,  y+2,maxx);
        prefresh(pads[y+index],   0,0, y,0,    y+1,maxx);
    }
    else if( (key == 'j') && (y == maxy - 1) ){

        ( (++index + (maxy - 1)) == SIZE) ? --index : index;

        if( (index + (maxy - 1)) < SIZE )
            refresh_pads_range(pads, index, maxy - 1, 1);
        move(maxy - 1,0);
    }
    else if( (key == 'h') && (x > 0) ){
        move(y, x - 1);
    }
    else if( (key == 'l') && (x < maxx) ){
        move(y, x + 1);
    }
}

