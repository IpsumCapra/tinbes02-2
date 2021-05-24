#include <constants.h>
#include <EEPROM.h>

#ifndef ARDUINOS_FILESYSTEM_H
#define ARDUINOS_FILESYSTEM_H

EERef noOfFiles = EEPROM[0];

struct FATEntry {
    char filename[NAMESIZE];
    int start;
    int length;
};

void setupFS();

bool writeFATEntry(FATEntry entry);
bool readFATEntry(int file, FATEntry& entry);
bool deleteFATEntry(int file);

int lookupEntry(char name[NAMESIZE]);

void defrag();

#endif //ARDUINOS_FILESYSTEM_H
