#include "directory.h"

static signed char DIR_X_addFileName(DIR_FileList *files, const char* fileName, size_t pos);
static char* DIR_X_fixPath(const char *path);
#ifdef _WIN32
static char* DIR_X_fixWindowsPath(const char *path);
#endif // _WIN32

DIR_FileList* DIR_newFileList(const char *path){

    DIR_FileList* data = malloc(sizeof(DIR_FileList));

    if(data == NULL){
        return NULL;
    }

    if(DIR_initFileList(data,path)==NULL){
        DIR_freeFileList(data);
        return NULL;
    }

    return data;
}


void* DIR_initFileList(DIR_FileList *files, const char *path){

    files->paths = NULL;
    files->length = 0;

    size_t i = 0;

#ifdef _WIN32

    char *dump = DIR_X_fixWindowsPath(path);
    const char* modPath = dump;
    if(modPath == NULL){
        modPath = path;
    }

    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    if((hFind = FindFirstFile(modPath, &fdFile)) == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    do {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0 && strcmp(fdFile.cFileName, "..") != 0)
        {
            /*
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            //Is the entity a File or Folder?
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                printf("Directory: %s\n", sPath);
                ListDirectoryContents(sPath); //Recursion, I love it!
            }
            else{
                printf("File: %s\n", sPath);
            }
            */
            if(DIR_X_addFileName(files,fdFile.cFileName,i)!=-1){
                i++;
                //printf("File: %s\n", fdFile.cFileName);
                //printf("File: %s\n", files->files[i-1]);
            }

        }
    } while(FindNextFile(hFind, &fdFile));

    FindClose(hFind);

    if(dump != NULL){
        free(dump);
    }

#else

    char *dump = DIR_X_fixPath(path);
    const char* modPath = dump;
    if(modPath == NULL){
        modPath = path;
    }

    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (modPath)) != NULL)
    {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL)
        {
            if(strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0)
            {
                if(DIR_X_addFileName(files,ent->d_name,i)!=-1){
                    i++;
                }
            }
        }
        closedir (dir);
    }
    else
    {
        if(dump != NULL){
            free(dump);
        }
        /* could not open directory */
        return NULL;
        //return EXIT_FAILURE;
    }

    if(dump != NULL){
        free(dump);
    }

#endif

    if(i==0){
        free(files->paths);
    }else{
        char** dat = realloc(files->paths,sizeof(char*)*i);
        if(dat!=NULL){
            files->paths=dat;
        }
    }
    files->length = i;

    return files;

}


///#ifdef _WIN32
char** DIR_getAllFiles(DIR_FileList *baseDirectory){

}
char** DIR_getDirectFiles(DIR_FileList *baseDirectory){
    //
}
//#endif






static signed char DIR_X_addFileName(DIR_FileList *files, const char* fileName, size_t pos){

    char** dat;

    if(pos >= files->length){
        if(files->length == 0){
            files->length=1;
            dat = malloc(sizeof(char*));
            if(dat==NULL){
                files->length = 0;
                return -1;
            }
            files->paths = dat;
        }else{
            dat = realloc(files->paths,sizeof(char*) * (files->length*2));
            if(dat == NULL){
                files->length = pos;
                return -1;
            }
            files->paths = dat;
            files->length*=2;
        }

    }

    size_t slen = strlen(fileName);

    files->paths[pos] = malloc(sizeof(char)*  (slen+1));
    if(files->paths[pos]==NULL){
        char** dat = realloc(files->paths,sizeof(char*)*(pos));
        if(dat!=NULL){
            files->paths = dat;
        }
        files->length = pos;
        return -1;
    }
    memcpy(files->paths[pos],fileName,sizeof(char)*(slen));
    files->paths[pos][slen] = '\0';

    return 0;
}

void DIR_freeFileList(DIR_FileList *files){
    DIR_freeFileListData(files);
    free(files);
}
void DIR_freeFileListData(DIR_FileList *files){

    size_t i;
    for(i=0; i < files->length; i++){
        free(files->paths[i]);
    }

    free(files->paths);

}







char DIR_exists(const char *path){

    char *dump = DIR_X_fixPath(path);
    const char* modPath = dump;
    unsigned char found = 0;
    if(modPath == NULL){
        modPath = path;
    }

#ifdef _WIN32
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    if((hFind = FindFirstFile(modPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        found = 0;
    }else{
        found = 1;
        FindClose(hFind);
    }

#else

    DIR *dir;
    if ((dir = opendir (modPath)) == NULL)
    {
        found = 0;
    }else{
        found = 1;
        closedir (dir);
    }


#endif

    if(dump != NULL){
        free(dump);
    }

    return found;
}
char DIR_isDirectory(const char *path){

    char *dump = DIR_X_fixPath(path);
    const char* modPath = dump;
    unsigned char isDir = 0;
    if(modPath == NULL){
        modPath = path;
    }

#ifdef _WIN32
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    if((hFind = FindFirstFile(modPath, &fdFile)) != INVALID_HANDLE_VALUE)
    {
        if( (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0){
            isDir = 1;
            FindClose(hFind);
        }
    }

#else

    struct stat statbuf;
    if (stat(modPath, &statbuf) == 0){
        isDir = (S_ISDIR(statbuf.st_mode) != 0);
    }


#endif

    if(dump != NULL){
        free(dump);
    }

    return isDir;
}


static char* DIR_X_fixPath(const char *path){

    size_t len = strlen(path);
    char *dump = NULL;

    if(path[len-1]=='\\' || path[len-1]=='/'){
        dump = malloc(sizeof(char)*(len));
        if(dump == NULL){
            return 0;
        }
        memcpy(dump, path, len*sizeof(char));
        dump[len-1]='\0';
        return dump;
    }

    return NULL;
}

#ifdef _WIN32
static char* DIR_X_fixWindowsPath(const char *path){

    size_t len = strlen(path);
    char *dump = NULL;
    unsigned char glob = 0;
    size_t i;

    for(i = len-1; i > 0; i--){
        if(path[i]=='*'){
            glob = 1;
            break;
        }
        if(path[i]=='\\' || path[i]=='/'){
            break;
        }
    }

    if(glob){
        return NULL;
    }



    if(path[len-1]=='\\' || path[len-1]=='/'){
        dump = malloc(sizeof(char)*(len+2));
        if(dump == NULL){
            return 0;
        }
        memcpy(dump, path, len*sizeof(char));
        dump[len]='*';
        dump[len+1]='\0';
        return dump;
    }else{
        dump = malloc(sizeof(char)*(len+3));
        memcpy(dump, path, len*sizeof(char));
        dump[len]='/';
        dump[len+1]='*';
        dump[len+2]='\0';
    }


    return dump;
}
#endif // _WIN32
