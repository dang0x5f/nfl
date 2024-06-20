#include <stdlib.h>
#include <ncurses.h>

#include "file_nodes.h"

// ncurses "KEY_ENTER" did not work
#define ENTER_KEY 10

void init(void);
void loop(void);

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
    index = 0;

    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    curs_set(0);
    keypad(stdscr,TRUE);
    refresh();
}

void loop(void){
    int key, quit = FALSE;

    getmaxyx(stdscr, maxy, maxx);

    fnode_t* filepads;    
    int f_cnt = populate_fnodes(&filepads, "/home/dang/");
    draw_fnodes(&filepads, f_cnt);

    do {
        noecho();
        key = getch();
        echo();

        switch(key){
            case ENTER_KEY:
                free_fnodes(&filepads, f_cnt);
                clear_screen();
                f_cnt = populate_fnodes(&filepads, "/home/dang/.vim/");
                draw_fnodes(&filepads, f_cnt);
                break;
            case 'q':
                quit = TRUE;
                break;
            case 'k':
            case 'j':
                move_cursor(key, &filepads, f_cnt);
                break;
            default:
                break;
        }

    } while(!quit);

    free_fnodes(&filepads, f_cnt);
}

void move_cursor(int key, fnode_t** pads, int f_cnt){

    getyx(stdscr, y, x);

    if(f_cnt <= maxy - 1)
        no_scroll(key, pads, f_cnt);
    else if(f_cnt > maxy - 1)
        with_scroll(key, pads, f_cnt);

            // debug //
    /* wbkgd((*pads)[1].fpad, COLOR_PAIR(1)); */
    /* prefresh((*pads)[1].fpad, 0,0, 1,0,  2,getmaxx(stdscr)); */
}

void no_scroll(int key, fnode_t** pads, int f_cnt){

    if ( (key == 'k') && (y > 0) ){
        move(y - 1, x);

        wbkgd( (*pads)[y - 1].fpad, COLOR_PAIR(1));
        wbkgd( (*pads)[y].fpad,     A_NORMAL);

        prefresh( (*pads)[y - 1].fpad,  0,0,  y-1,0,  y,maxx   );
        prefresh( (*pads)[y].fpad,      0,0,  y,0,    y+1,maxx );
    }
    else if( (key == 'j') && (y < f_cnt - 1) ){
        move(y + 1, x);

        wbkgd( (*pads)[y + 1].fpad, COLOR_PAIR(1));
        wbkgd( (*pads)[y].fpad,     A_NORMAL);

        prefresh( (*pads)[y + 1].fpad,  0,0,   y+1,0,  y+2,maxx );
        prefresh( (*pads)[y].fpad,      0,0,   y,0,    y+1,maxx );
    }
}

void with_scroll(int key, fnode_t** pads, int f_cnt){

    if( (key == 'k') && (y > 0) ){
        move(y - 1, x);

        wbkgd( (*pads)[y + index - 1].fpad, COLOR_PAIR(1) );
        wbkgd( (*pads)[y + index].fpad,     A_NORMAL      );

        prefresh( (*pads)[y + index - 1].fpad,  0,0,  y-1,0,  y,maxx   );
        prefresh( (*pads)[y + index].fpad,      0,0,  y,0,    y+1,maxx );
    }

    else if( (key == 'k') && (y == 0) ){
        ( (--index) < 0 ) ? index = 0 : index;
        if( (index) >= 0 )
            refresh_fnodes(pads, index, maxy - 1, 0);
        move(0,0);
    }

    else if( (key == 'j') && (y < maxy - 1) ){
        move(y + 1, x);
        wbkgd( (*pads)[y + index + 1].fpad, COLOR_PAIR(1) );
        wbkgd( (*pads)[y + index].fpad,     A_NORMAL      );

        prefresh( (*pads)[y + index + 1].fpad, 0,0, y+1,0,  y+2,maxx );
        prefresh( (*pads)[y + index].fpad,     0,0, y,0,    y+1,maxx );
    }
    else if( (key == 'j') && (y == maxy - 1) ){

        ( (++index + (maxy - 1)) == f_cnt) ? --index : index;

        if( (index + (maxy - 1)) < f_cnt )
            refresh_fnodes(pads, index, maxy - 1, 1);
        move(maxy - 1,0);
    }
}
