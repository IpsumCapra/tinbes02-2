#include <constants.h>

#ifndef ARDUINOS_FILESYSTEM_H
#define ARDUINOS_FILESYSTEM_H

struct FATEntry {
    char filename[NAMESIZE];
    int start;
    int length;
};

bool writeFATEntry(FATEntry entry);
bool readFATEntry(int file, FATEntry& entry);
bool deleteFATEntry(int file);
void defrag();
void setupFS();

#endif //ARDUINOS_FILESYSTEM_H
