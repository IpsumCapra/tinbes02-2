#include <constants.h>
#include <EEPROM.h>

#ifndef ARDUINOS_FILESYSTEM_H
#define ARDUINOS_FILESYSTEM_H

struct FATEntry {
    char filename[NAMESIZE];
    int start;
    int length;
};

void setupFS();

bool writeFATEntry(FATEntry entry);
bool readFATEntry(int file, FATEntry& entry);
bool updateFATEntry(int file, FATEntry entry);
bool deleteFATEntry(int file);

int lookupEntry(char name[NAMESIZE]);

bool storeFile(char name[NAMESIZE], int size);
bool retrieveFile(char name[NAMESIZE]);
bool deleteFile(char name[NAMESIZE]);

int freeSpace();

void listFiles();

#endif //ARDUINOS_FILESYSTEM_H
