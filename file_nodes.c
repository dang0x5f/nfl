#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "file_nodes.h"

int populate_fnodes(fnode_t** filepads, char* pwd){
    DIR *dir_ptr;    
    char path[1024];
    struct stat file_stat;
    struct dirent *entry_ptr;

    if((dir_ptr=opendir(pwd)) == NULL){
        fprintf(stderr, "opendir failed, %s\n", pwd);
        exit(EXIT_FAILURE);
    }

    int f_idx = 0;
    int f_cnt = 0;

    while( (entry_ptr=readdir(dir_ptr)) != NULL){
        strcpy(path,pwd);
        if(stat(strcat(path,entry_ptr->d_name), &file_stat) == -1){
            fprintf(stderr, "stat failed, %s \n%s %s\n", pwd, path, entry_ptr->d_name);
            exit(EXIT_FAILURE);
        }

        f_cnt++;

        if(f_cnt == 1)
            *filepads = malloc(sizeof(fnode_t));
        else
            *filepads = realloc(*filepads, sizeof(fnode_t) * f_cnt);

        (*filepads)[f_idx].fpad = newpad(1, getmaxx(stdscr));

        (*filepads)[f_idx].fname = malloc( sizeof(char) * (entry_ptr->d_namlen + 1) );
        strcpy( (*filepads)[f_idx].fname, entry_ptr->d_name );

        (*filepads)[f_idx].fperm[0]  = (S_ISDIR(file_stat.st_mode))  ? 'd' : '-'  ;
        (*filepads)[f_idx].fperm[1]  = (file_stat.st_mode & S_IRUSR) ? 'r' : '-'  ;
        (*filepads)[f_idx].fperm[2]  = (file_stat.st_mode & S_IWUSR) ? 'w' : '-'  ;
        (*filepads)[f_idx].fperm[3]  = (file_stat.st_mode & S_IXUSR) ? 'x' : '-'  ;
        (*filepads)[f_idx].fperm[4]  = (file_stat.st_mode & S_IRGRP) ? 'r' : '-'  ;
        (*filepads)[f_idx].fperm[5]  = (file_stat.st_mode & S_IWGRP) ? 'w' : '-'  ;
        (*filepads)[f_idx].fperm[6]  = (file_stat.st_mode & S_IXGRP) ? 'x' : '-'  ;
        (*filepads)[f_idx].fperm[7]  = (file_stat.st_mode & S_IROTH) ? 'r' : '-'  ;
        (*filepads)[f_idx].fperm[8]  = (file_stat.st_mode & S_IWOTH) ? 'w' : '-'  ;
        (*filepads)[f_idx].fperm[9]  = (file_stat.st_mode & S_IXOTH) ? 'x' : '-'  ;
        (*filepads)[f_idx].fperm[10] = '\0';

        f_idx++;
    }

    closedir(dir_ptr);
    return f_cnt;
}

void draw_fnodes(fnode_t** fpads, int count){
    for(int i = 0; i < count; i++){
        mvwprintw((*fpads)[i].fpad, 0,0, " %10s    %-20s", (*fpads)[i].fperm, (*fpads)[i].fname);
        if(i == 0)
            wbkgd((*fpads)[i].fpad, COLOR_PAIR(1));
        prefresh((*fpads)[i].fpad,  0,0,    i,0,    i+1,getmaxx(stdscr));
    }
}

void refresh_fnodes(fnode_t** fpads, int idx, int length, int direction){
    int maxx = getmaxx(stdscr); 


    if(direction == 0){
        clrtobot();
        wbkgd((*fpads)[idx].fpad,     COLOR_PAIR(1));
        wbkgd((*fpads)[idx + 1].fpad, A_NORMAL);
    }
    else if(direction == 1){
        int tempy, tempx;
        getyx(stdscr, tempy, tempx);
        move(0,0);
        clrtobot();
        // refresh() needs to occur LATER, before refreshing every pad
        move(tempy,tempx);

        wbkgd((*fpads)[idx + length].fpad,     COLOR_PAIR(1));
        wbkgd((*fpads)[idx + length - 1].fpad, A_NORMAL);
    }

    refresh();
    for (int i = 0, j = idx; i < getmaxy(stdscr); i++, j++)
        prefresh((*fpads)[j].fpad, 0,0, i,0, i+1, maxx);
}

void clear_screen(void){
    move(0,0);
    clrtobot();
    refresh();
}

void build_dirpath(char** path, char* dir){
    char* tmp;
    char* tmp2;

    tmp = malloc(sizeof(char) * (strlen(*path) + 1) );
    strcpy(tmp, *path);

    tmp2 = malloc(sizeof(char) * (strlen(dir) + 2) );
    strcpy(tmp2, dir);
    strcat(tmp2, "/");

    *path = realloc(*path, sizeof(char) * (strlen(*path) + strlen(tmp2) + 1) );
    strcpy(*path, tmp);
    strcat(*path, tmp2);

    free(tmp);
    free(tmp2);
}

void print_fnodes(fnode_t** filepads, int f_cnt){

    for(int i = 0; i < f_cnt; i++){

        printf("\nFile Info\n");
        printf("  %-5s%15s\n", "Name:", (*filepads)[i].fname);
        printf("  %-5s%15s\n", "Perm:", (*filepads)[i].fperm);
        printf("  %s\n", "--------------------");
    }

}

void free_fnodes(fnode_t** filepads, int f_cnt){
    for(int i = 0; i < f_cnt; i++){
        free( (*filepads)[i].fname );
    }
    free(*filepads);
}
