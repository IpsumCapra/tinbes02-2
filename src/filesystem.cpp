#include <filesystem.h>
#include <Arduino.h>
#include <EEPROM.h>

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
        EEPROM.put((noOfFiles - 1) * 16 + 1, entry);
        noOfFiles++;
        return true;
    }
    return false;
}

bool readFATEntry(int file, FATEntry& entry) {
    if (file <= noOfFiles) {
        entry = EEPROM.get((file - 1) * 16 + 1, entry);
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