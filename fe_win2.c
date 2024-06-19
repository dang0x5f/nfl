#include <stdlib.h>
#include <ncurses.h>

#include "file_nodes.h"

#define PWD "./"
/* #define SIZE 15 */
/* #define SIZE 48 */
/* #define SIZE 250 */

void init_prog(void);

void create_allpads(WINDOW**);
void refresh_allpads(WINDOW**);
void refresh_pads_range(WINDOW**,int,int,int);

void start_here(WINDOW*);

void move_cursor(int, WINDOW**);
void no_scroll(int, WINDOW**);
void with_scroll(int, WINDOW**);

static int maxy, maxx;
static int y, x;

static int index;

int main(void){
    int key, quit = FALSE;

    init_prog();

    /* WINDOW* pads[SIZE]; */
    /* create_allpads(pads); */
    /* refresh_allpads(pads); */

    /* fnode_t sumfile; */
    /* sumfile.fpad = newpad(1, 50); */
    /* sumfile.fname = "breezy.mp3"; */
    /* sumfile.fperm[0] = 'd'; */
    /* sumfile.fperm[1] = 'r'; */
    /* sumfile.fperm[2] = 'w'; */
    /* sumfile.fperm[3] = 'x'; */
    /* sumfile.fperm[4] = 'r'; */
    /* sumfile.fperm[5] = '-'; */
    /* sumfile.fperm[6] = '-'; */
    /* sumfile.fperm[7] = 'r'; */
    /* sumfile.fperm[8] = '-'; */
    /* sumfile.fperm[9] = '-'; */
    /* sumfile.fperm[10] = '\0'; */
    /* mvwprintw(sumfile.fpad, 0,0, "%-25s%25s", sumfile.fname, sumfile.fperm); */
    /* wbkgd(sumfile.fpad, COLOR_PAIR(1)); */
    /* prefresh(sumfile.fpad, 0,0,    0,3,    1,maxx); */


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
                move_cursor(key, pads);
                break;
            default:
                break;
        }

    } while(!quit);

    endwin();
    return(EXIT_SUCCESS);
}

void init_prog(void){
    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);
    getmaxyx(stdscr, maxy, maxx);
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

void move_cursor(int key, WINDOW* pads[]){
    getyx(stdscr, y, x);
    
    if(SIZE <= maxy - 1)
        no_scroll(key, pads);
    else if(SIZE > maxy - 1)
        with_scroll(key, pads);
}

void no_scroll(int key, WINDOW** pads){
    if ( (key == 'k') && (y > 0) ){
        move(y - 1, x);
        wbkgd(pads[y - 1], COLOR_PAIR(1));
        wbkgd(pads[y], A_NORMAL);

        prefresh(pads[y - 1],  0,0,  y-1,0,  y,maxx);
        prefresh(pads[y],      0,0,  y,0,    y+1,maxx);
    }
    else if( (key == 'j') && (y < SIZE - 1) ){
        move(y + 1, x);
        wbkgd(pads[y + 1], COLOR_PAIR(1));
        wbkgd(pads[y],     A_NORMAL);

        prefresh(pads[y + 1],  0,0,   y+1,0,  y+2,maxx);
        prefresh(pads[y],      0,0,   y,0,    y+1,maxx);
    }
}

void with_scroll(int key, WINDOW** pads){
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

