#include <filesystem.h>
#include <Arduino.h>
#include <EEPROM.h>

EERef noOfFiles = EEPROM[0];
int diskSize;

void setupFS() {
    diskSize = EEPROM.length();
    Serial.print(F("EEPROM size: "));
    Serial.println(diskSize);

    // Check if there is a FS structure available, create one if there isn't.
    if (noOfFiles > MAXFILES) {
        Serial.println(F("Creating filesystem."));
        noOfFiles = 0;
    } else if (noOfFiles > 0) {
        Serial.print(noOfFiles);
        Serial.println(F(" files found."));
    }
    Serial.println(F("Filesystem initialized."));
}

bool writeFATEntry(FATEntry entry) {
    if (noOfFiles < MAXFILES) {
        EEPROM.put(noOfFiles * 16 + 1, entry);
        noOfFiles++;
        return true;
    }
    return false;
}

bool readFATEntry(int file, FATEntry &entry) {
    if (file <= noOfFiles) {
        EEPROM.get((file - 1) * 16 + 1, entry);
        return true;
    }
    return false;
}

bool updateFATEntry(int file, FATEntry entry) {
    if (file <= noOfFiles) {
        EEPROM.put((file - 1) * 16 + 1, entry);
        return true;
    }
    return false;
}

bool deleteFATEntry(int file) {
    if (file <= noOfFiles) {
        for (int i = file; i < MAXFILES - 1; i++) {
            FATEntry temp;
            EEPROM.get((i) * 16 + 1, temp);
            EEPROM.put((i - 1) * 16 + 1, temp);
        }
        noOfFiles--;
        return true;
    }
    return false;
}

int lookupEntry(char name[NAMESIZE]) {
    for (int i = 1; i < MAXFILES + 1; i++) {
        FATEntry entry;
        readFATEntry(i, entry);
        if (strcmp(entry.filename, name) == 0) {
            return i;
        }
    }
    return -1;
}

int freeSpace() {
    FATEntry temp;
    int start = FATSIZE;
    if (noOfFiles > 0) {
        if (readFATEntry(noOfFiles, temp)) {
            start = temp.start + temp.length - 1;
        }
    }

    return diskSize - start;
}

bool storeFile(char name[NAMESIZE], int size) {
    if (size <= 0) {
        Serial.println(F("Invalid size."));
        return false;
    }
    if (lookupEntry(name) != -1) {
        Serial.println(F("File with specified name already exists."));
        return false;
    }

    if (noOfFiles == MAXFILES) {
        Serial.println(F("Maximum amount of files reached."));
        return false;
    }

    FATEntry temp;
    int start = FATSIZE;
    if (noOfFiles > 0) {
        if (readFATEntry(noOfFiles, temp)) {
            start = temp.start + temp.length;
        }
    }

    if (diskSize - start > size) {
        FATEntry newEntry;
        newEntry.start = start;
        newEntry.length = size;
        strcpy(newEntry.filename, name);
        writeFATEntry(newEntry);
    } else {
        Serial.println(F("Not enough space on disk."));
        return false;
    }

    Serial.print(F("Data: "));

    for (int i = 0; i < size; i++) {
        while (!Serial.available()) {}
        char input = Serial.read();
        Serial.print(input);
        EEPROM[start + i] = input;
    }

    Serial.println();
    return true;
}

bool retrieveFile(char name[NAMESIZE]) {
    int entry = lookupEntry(name);

    if (entry != -1) {
        FATEntry fEntry;
        if (readFATEntry(entry, fEntry)) {
            for (int i = 0; i < fEntry.length; i++) {
                char read = EEPROM[fEntry.start + i];
                Serial.print(read);
            }
        } else {
            Serial.println(F("Something went wrong."));
            return false;
        }

    } else {
        Serial.println(F("Entry not found."));
        return false;
    }
    Serial.println();
    return true;
}

bool deleteFile(char name[NAMESIZE]) {
    int file = lookupEntry(name);
    if (file == -1) {
        Serial.println(F("File does not exist."));
        return false;
    }

    if (file == noOfFiles) {
        deleteFATEntry(file);
    } else {
        FATEntry entry;
        if (readFATEntry(file, entry)) {
            FATEntry lastEntry;
            if(!readFATEntry(noOfFiles, lastEntry)) {
                Serial.println(F("Defragmentation failure."));
                return false;
            }
            for(int i = entry.start; i < lastEntry.start + lastEntry.length - entry.length; i++) {
                EEPROM[i] = EEPROM[i + entry.length];
            }
            for (int i = file + 1; i <= noOfFiles; i++) {
                FATEntry newEntry;
                if(!readFATEntry(i, newEntry)) {
                    Serial.println(F("FS Corrupt."));
                    return false;
                }
                newEntry.start -= entry.length;
                if(!updateFATEntry(i, newEntry)) {
                    Serial.println(F("Defragmentation failure."));
                    return false;
                }
            }
            deleteFATEntry(file);
        } else {
            Serial.println(F("Something went wrong."));
            return false;
        }

    }

    return true;
}

void listFiles() {
    for (int i = 1; i <= noOfFiles; i++) {
        FATEntry entry;
        readFATEntry(i, entry);
        Serial.print(F("File name: "));
        Serial.println(entry.filename);
        Serial.print(F("Starting position: "));
        Serial.println(entry.start);
        Serial.print(F("Size: "));
        Serial.print(entry.length);
        Serial.println(F(" bytes"));
    }
}
