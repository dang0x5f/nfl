#include <ncurses.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

#include "file_nodes.h"

int populate_fnodes(fnode_t** filepads){
    DIR *dir_ptr;    
    char path[50];
    struct stat file_stat;
    struct dirent *entry_ptr;

    const char* PWD = "./";

    if((dir_ptr=opendir(PWD)) == NULL){
        fprintf(stderr, "opendir failed\n");
        exit(EXIT_FAILURE);
    }

    int len = getmaxx(stdscr);
    /* getmaxx(stdscr, len); */

    int f_idx = 0;
    int f_cnt = 0;

    while( (entry_ptr=readdir(dir_ptr)) != NULL){
        strcpy(path,PWD);
        if(stat(strcat(path,entry_ptr->d_name), &file_stat) == -1){
            fprintf(stderr, "stat failed\n");
            exit(EXIT_FAILURE);
        }

        f_cnt++;

        if(f_cnt == 1)
            *filepads = malloc(sizeof(fnode_t));
        else
            *filepads = realloc(*filepads, sizeof(fnode_t) * f_cnt);

        /* filepads[f_idx].fpad = newpad(1,len); */
        printf("name:%s\nlength:%hu\n", entry_ptr->d_name, entry_ptr->d_namlen);

        (*filepads)[f_idx].fname = malloc( sizeof(char) * (entry_ptr->d_namlen + 1) );
        strcpy( (*filepads)[f_idx].fname, entry_ptr->d_name );

/*
        filepads[f_idx].fperm[0]  = (S_ISDIR(file_stat.st_mode))  ? 'd' : '-'  ;
        filepads[f_idx].fperm[1]  = (file_stat.st_mode & S_IRUSR) ? 'r' : '-'  ;
        filepads[f_idx].fperm[2]  = (file_stat.st_mode & S_IWUSR) ? 'w' : '-'  ;
        filepads[f_idx].fperm[3]  = (file_stat.st_mode & S_IXUSR) ? 'x' : '-'  ;
        filepads[f_idx].fperm[4]  = (file_stat.st_mode & S_IRGRP) ? 'r' : '-'  ;
        filepads[f_idx].fperm[5]  = (file_stat.st_mode & S_IWGRP) ? 'w' : '-'  ;
        filepads[f_idx].fperm[6]  = (file_stat.st_mode & S_IXGRP) ? 'x' : '-'  ;
        filepads[f_idx].fperm[7]  = (file_stat.st_mode & S_IROTH) ? 'r' : '-'  ;
        filepads[f_idx].fperm[8]  = (file_stat.st_mode & S_IWOTH) ? 'w' : '-'  ;
        filepads[f_idx].fperm[9]  = (file_stat.st_mode & S_IXOTH) ? 'x' : '-'  ;
        filepads[f_idx].fperm[10] = '\0';
*/

        f_idx++;
    }

    return f_cnt;
}


void print_fnodes(fnode_t* filepads, int f_cnt){

    for(int i = 0; i < f_cnt; i++){

        printf("\nFile Info\n");
        printf("  %-5s%15s\n", "Name:", filepads[i].fname);
        /* printf("  %-5s%15s\n", "Perm:", filepads[i].fperm); */
        printf("  %s\n", "--------------------");
    }

}

void free_fnodes(fnode_t* filepads, int f_cnt){
    for(int i = 0; i < f_cnt; i++){
        free(filepads[i].fname);
    }
    free(filepads);
}










/* 
 
             *** pseudo code ***

size = 1 
fnode_t farray = malloc(sizeof(fnode_t))

if fileptr NOT null
    
    farray[size - 1] = filevalues;     
   
    size++;
    farray = realloc(farray, sizeof(fnode_t) * size);
    farray + size = NULL;

endif


 */
