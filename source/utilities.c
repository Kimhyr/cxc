#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#include "utilities.h"

int fmmap(const char* restrict f, void* o)
{
    int fd = open(f, O_RDONLY);
    if (fd == -1) {
        perror(strerror(errno));
        exit(1);
    }

    struct stat fs;
    if (fstat(fd, &fs) == -1) {
        perror(strerror(errno));
        exit(1);
    }

    o = mmap(NULL, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (o == MAP_FAILED) {
        perror(strerror(errno));
        exit(1);
    }

    return fd;
}
