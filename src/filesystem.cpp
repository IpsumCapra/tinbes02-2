#include <filesystem.h>
#include <Arduino.h>
#include <EEPROM.h>

EERef noOfFiles = EEPROM[0];


void setupFS() {
    Serial.print("EEPROM size: ");
    Serial.println(EEPROM.length());

    // Check if there is a FS structure available, create one if there isn't.
    if (noOfFiles > 10) {
        Serial.println("Creating filesystem.");
        noOfFiles = 0;
    } else if (noOfFiles > 0) {
        Serial.print(noOfFiles);
        Serial.println(" files found.");
    }
    Serial.println("Filesystem initialized.");
}

bool writeFATEntry(FATEntry entry) {
    if (noOfFiles < 10) {
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
        for (int i = file; i < 9; i++) {
            FATEntry temp;
            EEPROM.get((i) * 16 + 1, temp);
            EEPROM.put((i - 1) * 16 + 1, temp);
        }
        noOfFiles--;
        return true;
    }
    return false;
}