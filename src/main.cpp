#include "api.h"
#include <iostream>

using namespace std;

int main() {
    const char *filename = "testfile.txt";

    HANDLE fd = lab2_open(filename);
    if (fd == NULL) {
        cerr << "Error: Unable to open file\n";
        return -1;
    }

    if (lab2_lseek(fd, 0, SEEK_END) == -1) {
        cerr << "Error, Unable to seek to end\n";
        lab2_close(fd);
        return -1;
    }

    const char *data = "New Data! ";
    if (lab2_write(fd, data, strlen(data)) < 0) {
        cerr << "Error: Write failed\n";
        return -1;
    }

    lab2_lseek(fd, 0, SEEK_SET);

    size_t buffer_len = lab2_read(fd, NULL, 0);
    char buffer[buffer_len] = {0};
    if (lab2_read(fd, buffer, sizeof(buffer)) < 0) {
        cerr << "Error: Read failed\n";
        return -1;
    }
    cout << "Read Data: " << buffer << endl;

    lab2_fsync(fd);
    lab2_close(fd);
    return 0;
}
