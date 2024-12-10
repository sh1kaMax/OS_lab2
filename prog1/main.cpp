#include <iostream>
#include <vector>
#include <cmath>
#include <windows.h>

#define BLOCK_SIZE 1024

using namespace std;

typedef HANDLE (*Lab2Open)(const char *);
typedef int (*Lab2Close)(HANDLE);
typedef ssize_t (*Lab2Write)(HANDLE, const void *, size_t);
typedef int (*Lab2Fsync)(HANDLE);

int main(int argc, char *argv[]) {
    HMODULE module = LoadLibrary((LPCSTR) "C:\\Users\\Max\\Dropbox\\OperationSystems\\lab2\\builddir\\liblab2_cache.dll");

    if (!module) {
        cerr << "Error: Cannot load the dynamic library!\n";
        return 1;
    }

    auto lab2_open = (Lab2Open)GetProcAddress(module, "lab2_open");
    auto lab2_close = (Lab2Close)GetProcAddress(module, "lab2_close");
    auto lab2_write = (Lab2Write)GetProcAddress(module, "lab2_write");
    auto lab2_fsync = (Lab2Fsync)GetProcAddress(module, "lab2_fsync");

    if (!lab2_open || !lab2_close || !lab2_write) {
        cerr << "Error: Cannot load required functions from library!\n";
        FreeLibrary(module);
        return 1;
    }

    if (argc != 2) {
        cerr << "Error: Invalid usage. Correct usage: ./prog.exe count\n";
        FreeLibrary(module);
        return 1;
    }

    size_t size_to_write = 100 * pow(2, 20);
    size_t written_size;
    vector<char> buffer(BLOCK_SIZE, 'F');

    HANDLE file = lab2_open("output.bin");

    if (file == INVALID_HANDLE_VALUE) {
        cerr << "Error: Cannot open file!\n";
        FreeLibrary(module);
        return 1;
    }

    for (int i = 0; i < atoi(argv[1]); i++) {
        written_size = 0;
        while (written_size < size_to_write) {
            ssize_t bytes_written = lab2_write(file, buffer.data(), BLOCK_SIZE);
            if (bytes_written <= 0) {
                cerr << "Error: WriteFile!\n";
                lab2_close(file);
                FreeLibrary(module);
                return 1;
            }
            written_size += bytes_written;
        }
    }
    lab2_fsync(file);
    lab2_close(file);
    FreeLibrary(module);
    return 0;
}
