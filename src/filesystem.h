#include <constants.h>

#ifndef ARDUINOS_FILESYSTEM_H
#define ARDUINOS_FILESYSTEM_H

struct FATEntry {
    char filename[NAMESIZE];
    int start;
    int length;
};

bool writeFATEntry(FATEntry entry);
FATEntry readFATEntry(int start);

#endif //ARDUINOS_FILESYSTEM_H
