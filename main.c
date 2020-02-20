#include <stdio.h>
#include <stdlib.h>

#include "directory.h"

int main()
{
    printf("Hello world!\n");

    /*
    DIR_FileList list;
    DIR_initFileList(&list,"C:/*");


    if(list.files != NULL){
        size_t i;
        for(i=0;i< list.length;i++){
            puts(list.files[i]);
        }
    }

    DIR_freeFileListData(&list);
    /*/

    printf("IsDir: %i\n",DIR_isDirectory("C:/"));
    printf("IsDir: %i\n",DIR_isDirectory("C:"));
    printf("Exists: %i\n",DIR_exists("C:/"));
    printf("Exists: %i\n",DIR_exists("C:"));
    puts("\n\n\n");


    DIR_FileList *list = DIR_newFileList("C:/");

    if(list != NULL){
        size_t i;
        for(i=0;i< list->length;i++){
            puts(list->paths[i]);
        }
        DIR_freeFileList(list);
    }

    //*/

    return 0;
}
