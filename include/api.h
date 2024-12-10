#ifndef API_H
#define API_H

#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <_mingw_off_t.h>

using namespace std;
extern "C" {
    HANDLE lab2_open(const char *);
    int lab2_close(HANDLE);
    ssize_t lab2_read(HANDLE, void *, size_t);
    ssize_t lab2_write(HANDLE, const void *, size_t);
    off_t lab2_lseek(HANDLE, off_t, int);
    int lab2_fsync(HANDLE);
}

#endif