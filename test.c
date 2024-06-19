#include <stdlib.h>
#include <ncurses.h>

#include "file_nodes.h"

int main(void){
    int nfiles;
    fnode_t* files;

    nfiles = populate_fnodes(&files);

    printf("\n%d\n", nfiles);
    printf("\n%s\n", files[0].fname);

    /* print_fnodes(files, nfiles); */    
    /* free_fnodes(files, nfiles); */    

    return(EXIT_SUCCESS);
}
