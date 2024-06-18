#include <stdlib.h>
#include <ncurses.h>

void move_cursor(int, WINDOW**);

static int maxy, maxx;
static int y, x;

int main(void){
    int key, quit = FALSE;

    initscr();
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_RED);

    getmaxyx(stdscr, maxy, maxx);

    WINDOW* subwins[maxy];

    for(int i = 0; i < maxy; i++){
        subwins[i] = derwin(stdscr, 1, maxx, i, 0);
        /* if( (i % 2) == 0){ */
        /*     wbkgd(subwins[i], COLOR_PAIR(1) ); */
        /* } */
        mvwprintw(subwins[i], 0, 15, "%d", i);
    }
        mvwprintw(subwins[0], 0, 25, "%d", maxy);
    wbkgd(subwins[0], COLOR_PAIR(1));

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
                move_cursor(key, subwins);
                break;
            default:
                break;
        }

    } while(!quit);

    endwin();
    return(EXIT_SUCCESS);
}

void move_cursor(int key, WINDOW* subwins[]){
    getyx(stdscr, y, x);

    if( (key == 'k') && (y > 0) ){
        move(y - 1, x);
        wbkgd(subwins[y-1], COLOR_PAIR(1));
        wbkgd(subwins[y], A_NORMAL);
        wrefresh(subwins[y-1]);
        wrefresh(subwins[y]);
    }
    else if( (key == 'j') && (y < maxy - 1) ){
        move(y + 1, x);
        wbkgd(subwins[y + 1], COLOR_PAIR(1));
        wbkgd(subwins[y], A_NORMAL);
        wrefresh(subwins[y + 1]);
        wrefresh(subwins[y]);
    }
    else if( (key == 'h') && (x > 0) ){
        move(y, x - 1);
    }
    else if( (key == 'l') && (x < maxx) ){
        move(y, x + 1);
    }

}
