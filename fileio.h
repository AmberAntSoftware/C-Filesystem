#ifndef FILEIO_H_INCLUDED
#define FILEIO_H_INCLUDED

#ifdef _WIN32

    #define FIO_BUFFER_SIZE 4096

    #include <windows.h>

#else

    #define FIO_BUFFER_SIZE 4096

    #include <dirent.h>
    #include <sys/stat.h>
    #include <unistd.h>

#endif

typedef struct FIO_FileBuffer{
    /*
    custom file handler
    */
    unsigned char *buffer;
    size_t buffer_size;
}FIO_FileBuffer;

FIO_FileBuffer* FIO_newFileBuffer(const char* path);
void* FIO_initFileBuffer(FIO_FileBuffer* buff, const char* path, size_t bufferSize);
void FIO_freeFileBuffer(FIO_FileBuffer* buff);
void FIO_freeFileBufferData(FIO_FileBuffer* buff);

#endif // FILEIO_H_INCLUDED
