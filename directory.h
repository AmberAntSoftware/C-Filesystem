#ifndef DIRECTORY_H_INCLUDED
#define DIRECTORY_H_INCLUDED

#include <string.h>
#include <stdlib.h>

//#undef _WIN32

#ifdef _WIN32

    #include <windows.h>

#else

    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>

#endif

typedef struct DIR_FileList{

    char** paths;

    size_t length;
    size_t dirCount;

}DIR_FileList;


DIR_FileList* DIR_newFileList(const char *path);
void* DIR_initFileList(DIR_FileList *files, const char *path);
void DIR_freeFileList(DIR_FileList *files);
void DIR_freeFileListData(DIR_FileList *files);

unsigned char DIR_exists(const char *path);
unsigned char DIR_isDirectory(const char *path);
size_t DIR_fileCount(const char *path);

//////////////////////

char** DIR_getAllFiles(DIR_FileList *baseDirectory);
char** DIR_getDirectFiles(DIR_FileList *baseDirectory);


#endif // DIRECTORY_H_INCLUDED
