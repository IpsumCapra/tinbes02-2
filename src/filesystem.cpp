#include <filesystem.h>
#include <Arduino.h>
#include <EEPROM.h>

EERef noOfFiles = EEPROM[0];
int diskSize;

void setupFS() {
    diskSize = EEPROM.length();
    Serial.print("EEPROM size: ");
    Serial.println(diskSize);

    // Check if there is a FS structure available, create one if there isn't.
    if (noOfFiles > MAXFILES) {
        Serial.println("Creating filesystem.");
        noOfFiles = 0;
    } else if (noOfFiles > 0) {
        Serial.print(noOfFiles);
        Serial.println(" files found.");
    }
    Serial.println("Filesystem initialized.");
}

bool writeFATEntry(FATEntry entry) {
    if (noOfFiles < MAXFILES) {
        Serial.println(entry.filename);
        Serial.println(entry.start);
        Serial.println(entry.length);
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
    if (lookupEntry(name) != -1) {
        Serial.println("File with specified name already exists.");
        return false;
    }

    if (noOfFiles == MAXFILES) {
        Serial.println("Maximum amount of files reached.");
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
        Serial.println("Not enough space on disk.");
        return false;
    }

    Serial.print("Data: ");

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
        if (!readFATEntry(entry, fEntry)) {
            Serial.println("Something went wrong.");
            return false;
        } else {
            for (int i = 0; i < fEntry.length; i++) {
                char read = EEPROM[fEntry.start + i];
                Serial.print(read);
            }
        }

    } else {
        Serial.println("Entry not found.");
        return false;
    }
    Serial.println();
    return true;
}

bool deleteFile(char name[NAMESIZE]) {

}

void listFiles() {
    for (int i = 1; i <= noOfFiles; i++) {
        FATEntry entry;
        readFATEntry(i, entry);
        Serial.print("File name: ");
        Serial.println(entry.filename);
        Serial.print("Starting position: ");
        Serial.println(entry.start);
        Serial.print("Size: ");
        Serial.print(entry.length);
        Serial.println(" bytes");
    }
}
