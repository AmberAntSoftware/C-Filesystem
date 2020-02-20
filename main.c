#include <stdio.h>
#include <stdlib.h>

#include "directory.h"

int main()
{

    printf("IsDir: %i\n",DIR_isDirectory("C:/"));
    printf("IsDir: %i\n",DIR_isDirectory("C:"));
    printf("Exists: %i\n",DIR_exists("C:/"));
    printf("Exists: %i\n",DIR_exists("C:"));
    printf("Count: %u\n",(unsigned int)DIR_fileCount("C:/"));
    printf("Count: %u\n",(unsigned int)DIR_fileCount("C:"));
    puts("\n\n\n");


    DIR_FileList *list = DIR_newFileList("C:/Users");

    if(list != NULL){
        size_t i;
        for(i=0;i< list->length;i++){
            puts(list->paths[i].path);
        }
        DIR_freeFileList(list);
    }

    return 0;
}
